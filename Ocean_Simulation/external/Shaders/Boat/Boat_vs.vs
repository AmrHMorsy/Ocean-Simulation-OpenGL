#version 330 core


layout (location = 0) in vec3 vertex ;
layout (location = 1) in vec2 _texture ;
layout (location = 2) in vec3 __normal ;

out vec3 _normal ;
out vec3 view_position ;
out vec3 world_position ;
out vec2 texture_coordinates ;
out vec4 fragment_position_light_space ;

uniform mat4 model ;
uniform mat4 view ;
uniform mat4 projection ;
uniform sampler2D displacement_map ; 
uniform mat4 light_view ;
uniform mat4 light_projection ;


void main()
{
    texture_coordinates = _texture ;
    _normal = __normal ;
    vec4 position = vec4(vertex, 1.0);
    world_position = (model * position).xyz ;
    
    fragment_position_light_space = light_projection * light_view * vec4(world_position, 1.0) ;
    
    gl_Position = projection * view * vec4(world_position, 1.0);
    view_position = (view * vec4(world_position, 1.0)).xyz;
}
