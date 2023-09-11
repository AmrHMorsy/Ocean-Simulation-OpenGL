#version 330 core

layout (location = 0) in vec3 vertex ;
layout (location = 1) in vec2 _texture ;
layout (location = 2) in vec3 __normal ;
layout (location = 3) in float index ;
layout (location = 4) in mat4 model ;

out vec2 texture_coordinates ;
out vec3 world_position ;
out float mirror ;


void calculate_texture_coordinates()
{
    mirror = 1.0f ;
    texture_coordinates = _texture ;
    if (index == 1){
        texture_coordinates.x = 1.0 - _texture.x ;
        mirror = 1.0f ;
    }
    else if (index == 2){
        texture_coordinates.y = 1.0 - _texture.y ;
        mirror = -1.0f ;
    }
    else if (index == 3){
        texture_coordinates.x = 1.0 - _texture.x ;
        texture_coordinates.y = 1.0 - _texture.y ;
        mirror = -1.0f ;
    }
}

void main()
{
    world_position = (model * vec4(vertex, 1.0)).xyz ;
    calculate_texture_coordinates() ;
}
