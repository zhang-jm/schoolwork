#version 330 core

out vec4 color;

in vec3 n;

void main()
{
	color = vec4(n.x, n.y, n.z, 1.0f);
}