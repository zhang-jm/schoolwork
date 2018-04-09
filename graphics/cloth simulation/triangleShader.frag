#version 330 core

in float sampleExtraOutput;
in vec3 normal;

out vec4 color;

void main()
{
    color = vec4(0.5f, 0.5f, abs(normal.z), sampleExtraOutput);
}
