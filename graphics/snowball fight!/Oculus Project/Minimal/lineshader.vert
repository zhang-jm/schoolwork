#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;

void main()
{
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
}

