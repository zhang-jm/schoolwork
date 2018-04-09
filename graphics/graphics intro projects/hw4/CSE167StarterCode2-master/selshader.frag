#version 330 core
// This is a sample fragment shader.

out vec4 color;

uniform int id;

void main()
{
	color = vec4(id / 255.0f, 0.0f, 0.0f, 0.0f);
}
