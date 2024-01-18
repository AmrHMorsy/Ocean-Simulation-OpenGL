#version 330 core

out vec4 FragColor;
in vec3 TexCoords;
in vec3 world_position;

uniform vec3 camera;
uniform samplerCube skybox;

float compute_fog()
{
    float fog_density = 0.5f ;
    float distance = 4.0f *length(world_position - camera);
    float vertical_gradient = 1.0 - clamp(world_position.y, 0.0, 1.0) ;
    vertical_gradient = vertical_gradient * 0.8 ;
    float fog = exp(-pow(fog_density * distance * vertical_gradient, 2));
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}

//float compute_fog()
//{
//    float fog_density = 0.5f ;
//    float distance = length(world_position - camera);
//    float fog = exp(-pow(fog_density * distance, 2)) ;
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}

//float compute_fog()
//{
//    float fog_density = 0.5f ;
//    float distance = 2*length(world_position - camera);
//    float fog = exp(-pow(fog_density * distance, 2));
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}

void main()
{
    vec4 color = texture(skybox, TexCoords);
    color.rgb = color.rgb * pow(2.0, -2.0f);
    color.rgb = color.rgb / (color.rgb + vec3(1.0));
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
//    FragColor = color ;
    FragColor = mix(vec4(0.5f, 0.5f, 0.5f, 1.0),color,compute_fog()) ;
}
