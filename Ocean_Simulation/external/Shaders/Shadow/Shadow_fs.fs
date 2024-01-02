#version 330 core

out vec4 FragColor;

in vec4 gl_FragCoord ;

void main()
{
    gl_FragDepth = gl_FragCoord.z;

    FragColor = vec4( vec3(gl_FragCoord.z), 1.0f) ;
}
