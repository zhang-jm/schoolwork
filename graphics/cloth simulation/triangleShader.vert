#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 projection;
uniform mat4 modelview;

out float sampleExtraOutput;
out vec3 normal;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    sampleExtraOutput = 1.0f;
	normal = normals;
}
