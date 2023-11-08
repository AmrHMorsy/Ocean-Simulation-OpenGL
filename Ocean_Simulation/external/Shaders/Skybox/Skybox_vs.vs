#version 330 core


layout (location = 0) in vec3 aPos;
out vec3 TexCoords;


uniform mat4 projection;
uniform mat4 view;

out vec3 world_position ;

void main()
{
    TexCoords = aPos;
    mat4 v = mat4(mat3(view)) ;
    vec4 pos = projection * v * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    world_position = aPos ; 
}  
