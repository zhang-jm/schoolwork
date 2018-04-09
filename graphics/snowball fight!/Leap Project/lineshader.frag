#version 330 core

uniform float pressed;

layout(location = 0) out vec4 color;

void main()
{    
	if(pressed == 1.0f) {
	   color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} 
	else {
	   color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}
