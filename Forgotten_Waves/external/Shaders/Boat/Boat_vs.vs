#version 330 core

layout (location = 0) in vec3 vertex ;
layout (location = 1) in vec2 _texture ;
layout (location = 2) in vec3 __normal ;


out vec2 texture_coordinates ;
out vec3 _normal ;
out vec3 view_position ;
out vec3 world_position ;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model ;



void main()
{
    gl_Position = model * vec4(vertex, 1.0) ;
    world_position = gl_Position.xyz ;
    gl_Position = view * gl_Position ;
    view_position = gl_Position.xyz ;
    gl_Position = projection * gl_Position ;
    texture_coordinates = vec2(_texture.x, _texture.y);
    _normal = __normal ;
}
