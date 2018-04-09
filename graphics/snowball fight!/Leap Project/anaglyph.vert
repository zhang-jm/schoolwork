#version 330 core

layout (location = 0) in vec3 position;

out vec2 TexCoords;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    TexCoords = (position.xy+vec2(1,1))/2.0;
}