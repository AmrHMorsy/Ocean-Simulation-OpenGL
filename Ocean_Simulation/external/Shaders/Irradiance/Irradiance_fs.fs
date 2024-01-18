#version 330 core

in vec3 localPos ;

out vec4 FragColor ;

uniform samplerCube irradiance_map ;

const float PI = 3.14159265359;

vec3 ToneMap_Reinhard(vec3 color)
{
    color = color / (color + vec3(1.0));
    return color;
}

vec3 calculate_irradiance()
{
    vec3 normal = normalize(localPos);
    vec3 irradiance = vec3(0.0) ;
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));
    float nrSamples = 0.0 ;
    float sampleDelta = 0.02 ;
    for( float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta ){
        for( float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta ){
            float cos_theta = cos(theta) ;
            float sin_theta = sin(theta) ;
            float cos_phi = cos(phi) ;
            float sin_phi = sin(phi) ;
            vec3 tangentSample = vec3(sin_theta*cos_phi, sin_theta*sin_phi , cos_theta ) ;
            vec3 sampleVec = (tangentSample.x * right) + (tangentSample.y * up) + (tangentSample.z * normal) ;
            vec3 irr = texture(irradiance_map, sampleVec).rgb ;
            irr = ToneMap_Reinhard(irr) ;
            irr = irr * cos_theta * sin_theta ;
            irradiance += irr ;
            nrSamples++;
        }
    }
    irradiance = PI * irradiance ;
    irradiance = irradiance * (1.0 / float(nrSamples)) ;
    return irradiance ;
}

void main()
{
    vec3 irradiance = calculate_irradiance() ;
    FragColor = vec4(irradiance, 1.0);
}
