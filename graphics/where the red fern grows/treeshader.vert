#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;

out vec3 n;

void main()
{
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);

	n = normal;
}
