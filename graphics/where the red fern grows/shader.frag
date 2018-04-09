#version 330 core

out vec4 color;

in float pos;
in vec3 n;

void main()
{
	if(pos < -5.0f){
		color = vec4(n.x * 1.5, n.x * 1.5, n.x * 1.5, 1.0f);
	}
	else{
		if(pos < 0.0f) {
		   color = vec4(n.x, 0.0f, n.y/2, 1.0f);
		}
		else{	
			color = vec4(n.x * 5, 0.0f, n.z / 2, 1.0f);
		}
	}
}