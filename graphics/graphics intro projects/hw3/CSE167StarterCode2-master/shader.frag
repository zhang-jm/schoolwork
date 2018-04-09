#version 330 core
// This is a sample fragment shader.

out vec4 color;

in float c;

void main()
{
	if(c == 1){
		color = vec4(0.1f, 0.7f, 0.3f, 0.5f);
	}

	else{
		color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	}
}
