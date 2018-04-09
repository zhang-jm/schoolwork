#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;

out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    
	Normal = mat3(transpose(inverse(model))) * normal;
    Position = vec3(model * vec4(position, 1.0f));
}
