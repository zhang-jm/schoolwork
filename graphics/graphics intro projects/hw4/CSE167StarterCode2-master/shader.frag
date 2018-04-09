#version 330 core
// This is a sample fragment shader.

uniform int color;
out vec4 fragColor;


uniform int id;

void main()
{
	if(color == 1){
		fragColor = vec4(0.1f, 0.7f, 0.3f, 0.5f);
	}

	else if(color == 2){
		fragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f);
	}

	else if(color == 3){
		fragColor = vec4(0.0f, 0.0f, 1.0f, 0.5f);
	}
		
	else if(color == 4){
		fragColor = vec4(1.0f, 1.0f, 0.0f, 0.5f);
	}

	else{
		fragColor = vec4(id / 255.0f, 0.0f, 1.0f, 0.0f);
	}
}
