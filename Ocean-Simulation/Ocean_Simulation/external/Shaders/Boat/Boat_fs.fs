#version 330 core

in vec2 texture_coordinates ;
in vec3 _normal ;
in vec3 view_position ;
in vec3 world_position ;

out vec4 fragment ;

uniform sampler2D albedo_map ;
uniform sampler2D ao_map ;
uniform sampler2D normal_map ;
uniform sampler2D roughness_map ;
uniform sampler2D displacement_map ;
uniform samplerCube irradiance_map ;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_integration_map ;

uniform vec3 camera ;
uniform vec3 light_radiance ;
uniform vec3 light_color ;
uniform vec3 lights[2] ;
uniform float x ;

const float PI = 3.14159265359 ;



vec3 gamma_correct( vec3 color )
{
    color = pow(color, vec3(1.0/2.2)) ;
    
    return color ;
}

vec3 HDR_tonemapping( vec3 color )
{
    color = color / (color + vec3(1.0));
    
    return color ;
}

vec3 calculate_normal()
{
    vec3 tangent_normal = texture(normal_map, texture_coordinates).xyz * 2.0 - 1.0 ;
    vec3 Q1  = dFdx(world_position) ;
    vec3 Q2  = dFdy(world_position);
    vec2 st1 = dFdx(texture_coordinates);
    vec2 st2 = dFdy(texture_coordinates);
    vec3 N   = normalize(_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize( TBN * tangent_normal );
}

float geometry_schlick( float dot_product, float roughness )
{
    float r = (roughness + 1.0) ;
    float k_direct = (r*r) / 8.0 ;
    float numerator = dot_product ;
    float denominator = dot_product * (1.0-k_direct) + k_direct  ;
    
    return numerator / denominator ;
}

float geometry_smith( vec3 normal, vec3 view, vec3 light, float roughness )
{
    float normal_dot_view = max(dot(normal, view), 0.0);
    float normal_dot_light = max(dot(normal, light), 0.0);

    return geometry_schlick(normal_dot_light,roughness) * geometry_schlick(normal_dot_view,roughness) ;
}

float reitz_ndf( vec3 normal, vec3 halfway, float roughness )
{
    float r = roughness * roughness ;
    float a = r * r ;
    float normal_dot_halfway = max( dot(normal,halfway), 0.0 ) ;
    float normal_dot_halfway_sq = normal_dot_halfway * normal_dot_halfway ;
    float numerator = a ;
    float denominator = (normal_dot_halfway_sq * (a - 1.0) + 1.0) ;
    denominator = PI * denominator * denominator ;
    
    return numerator / denominator ;
}

vec3 frensel_schlick( float cos_theta, vec3 albedo, float metallic )
{
    vec3 base_reflectivity = vec3(0.04) ;
    base_reflectivity = mix(base_reflectivity, albedo, metallic) ;
    vec3 ratio = base_reflectivity + (1.0 - base_reflectivity) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);
    
    return ratio ;
}

vec3 cook_torrance_brdf( vec3 normal, vec3 halfway, vec3 view, vec3 light, vec3 albedo, float roughness, float metallic )
{
    vec3 lambertian_diffuse = albedo / PI ;
    vec3 frensel = frensel_schlick( max(dot(halfway,view),0.0), albedo, metallic ) ;
    vec3 ks = frensel ;
    vec3 kd = vec3(1.0) - ks ;
    kd *= 1.0 - metallic;
    float ndf = reitz_ndf(normal, halfway, roughness) ;
    float geometry = geometry_smith(normal, view, light, roughness) ;
    float normal_dot_view = max( dot(normal, view), 0.0 );
    float normal_dot_light = max( dot(normal, light), 0.0 );
    vec3 specular = (ndf*geometry*frensel) / ((4.0*normal_dot_view*normal_dot_light)+0.0001 ) ;
    vec3 brdf = ( kd * lambertian_diffuse ) + ( ks * specular ) ;
    
    return brdf ;
}

vec3 calculate_radiance(vec3 normal, vec3 view, vec3 albedo, float roughness, float metallic)
{
    float cos_theta ;
    vec3 light, halfway, brdf, out_radiance = vec3(0.0) ;
    for( int i = 0 ; i < 1 ; i++ ){
        light = normalize(lights[i]) ;
        halfway = normalize(view + light) ;
        cos_theta = max(dot(normal, light), 0.0) ;
        brdf = cook_torrance_brdf(normal, halfway, view, light, albedo, roughness, metallic) ;
        out_radiance += brdf * light_radiance * cos_theta;
    }

    return out_radiance;
}

vec3 fresnelSchlickRoughness(float cosTheta, float roughness, vec3 albedo, float metallic )
{
    vec3 F0 = vec3(0.04) ;
    F0 = mix(F0, albedo, metallic) ;
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calculate_ambient( vec3 normal, vec3 view, vec3 albedo, float ao, float roughness, float metallic )
{
    vec3 kS = fresnelSchlickRoughness(max(dot(normal, view), 0.0), roughness, albedo, metallic);
    vec3 kD = 1.0 - kS ;
    vec3 irradiance = texture(irradiance_map, normal).rgb ;
    vec3 diffuse = irradiance * albedo ;
    vec3 R = reflect(-view, normal);
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdf_integration_map, vec2(max(dot(normal, view), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);
    vec3 ambient = (kD * diffuse + specular) * ao ;
    
    return ambient ;
}

vec4 pbr()
{
    vec3 displacement = texture(displacement_map, texture_coordinates).rgb;
    float metallic  = texture(displacement_map, texture_coordinates).r;
    vec3 albedo = texture(albedo_map, texture_coordinates).rgb ;
    albedo = vec3(pow(albedo.r, 2.2), pow(albedo.g, 2.2), pow(albedo.b, 2.2));
    float roughness = 0.98 ;
    float ao = texture(ao_map, texture_coordinates).r ;
    vec3 normal = calculate_normal() ;
    vec3 view = normalize(camera - world_position) ;
    vec3 radiance = calculate_radiance( normal, view, albedo, roughness, metallic) ;
    vec3 ambient = calculate_ambient( normal, view, albedo, ao, roughness, metallic ) ;
    vec3 color = ambient + radiance ;
    color = HDR_tonemapping(color) ;
    color = gamma_correct(color) ;
    
    return vec4( color, 1.0 ) ;
}

void main()
{
    fragment = pbr() ;
}
