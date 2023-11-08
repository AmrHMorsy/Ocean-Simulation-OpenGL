#version 330 core

in vec3 localPos ;

out vec4 fragment ;

uniform float roughness ;
uniform samplerCube environment_map ;

const uint samples = 16384u ;

const float PI = 3.14159265359 ;


float reitz_ndf( vec3 normal, vec3 halfway )
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

float Van_Der_Corput_sequence( uint bits )
{
     bits = (bits << 16u) | (bits >> 16u) ;
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u) ;
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u) ;
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u) ;
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u) ;
     
    return float(bits) * 2.3283064365386963e-10 ;
}

vec2 Hammersley( uint i, uint samples )
{
    float low_discrepancy_sample = Van_Der_Corput_sequence(i) ;
    vec2 hammersley = vec2( float(i)/float(samples), low_discrepancy_sample ) ;
    
    return hammersley ;
}

vec3 importance_sampling_GGX( vec2 hammersley, vec3 normal )
{
    vec3 halfway ;
    float a = roughness * roughness ;
    float phi = 2.0 * PI * hammersley.x ;
    float cos_theta = sqrt((1.0 - hammersley.y) / (1.0 + (a*a - 1.0) * hammersley.y)) ;
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta) ;
    halfway.x = cos(phi) * sin_theta ;
    halfway.y = sin(phi) * sin_theta ;
    halfway.z = cos_theta ;
    vec3 up = (abs(normal.z) < 0.999) ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);
    vec3 sample_vector = (tangent * halfway.x) + (bitangent * halfway.y) + (normal * halfway.z) ;
    
    return normalize(sample_vector);
}

float calculate_mip_level( float pdf )
{
    float resolution = 512.0 ;
    float saTexel  = 4.0 * PI / (6.0 * resolution * resolution) ;
    float saSample = 1.0 / (float(samples) * pdf + 0.0001) ;
    float mip_level = (roughness == 0.0) ? 0.0 : (0.5 * log2(saSample / saTexel)) ;
    
    return mip_level ;
}

vec3 sample_halfway( uint i, vec3 normal )
{
    vec2 hammersley = Hammersley(i, samples) ;
    vec3 halfway = importance_sampling_GGX( hammersley , normal ) ;
    
    return halfway ;
}

float calculate_prob_dist_funct( vec3 normal, vec3 halfway, float normal_dot_halfway, float halfway_dot_view )
{
    float ndf   = reitz_ndf(normal, halfway) ;
    float pdf = ndf * normal_dot_halfway / (4.0 * halfway_dot_view) + 0.0001 ;
    
    return pdf ;
}

void main()
{
    vec3 color = vec3(0.0) ;
    vec3 normal = normalize(localPos) ;
    vec3 view = normal ;
    float total_weight = 0.0 ;
    
    for( uint i = 0u ; i < samples ; ++i ){
        vec3 halfway = sample_halfway(i, normal) ;
        vec3 light  = normalize(2.0 * dot(view, halfway) * halfway - view) ;
        float normal_dot_light = max(dot(normal, light), 0.0) ;
        if( normal_dot_light > 0.0 ){
            float normal_dot_halfway = max(dot(normal, halfway), 0.0) ;
            float halfway_dot_view = max(dot(halfway, view), 0.0) ;
            float pdf = calculate_prob_dist_funct(normal, halfway, normal_dot_halfway, halfway_dot_view ) ;
            float mip_level = calculate_mip_level( pdf ) ;
            color += textureLod(environment_map, light, mip_level).rgb * normal_dot_light ;
            total_weight += normal_dot_light ;
        }
    }
    color = color / total_weight ;
    fragment = vec4(color, 1.0);
}
