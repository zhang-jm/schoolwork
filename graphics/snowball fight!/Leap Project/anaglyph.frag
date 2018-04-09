#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform int hit;
uniform sampler2D lefteye;
uniform sampler2D righteye;

void main()
{   
	vec4 leftFrag = texture2D(lefteye, TexCoords);
	leftFrag = vec4(leftFrag.r, 1.0, 1.0, 1.0); // Left eye is full green and blue and actual red
 
	vec4 rightFrag = texture2D(righteye, TexCoords);
	rightFrag = vec4(1.0, rightFrag.g, rightFrag.b, 1.0); // Right eye is full red and actual green and blue
 
	// Multiply left and right components for final output colour
	//color = leftFrag;
	//color = rightFrag;
    if(hit == 1) {
	   color = vec4(1.0f, leftFrag.gb * rightFrag.gb, 1.0);
	}
	else if(hit == 2){
	   color = vec4(leftFrag.r * rightFrag.r, 1.0f, leftFrag.b * rightFrag.b, 1.0);
	}
	else {
	   color = vec4(leftFrag.rgb * rightFrag.rgb, 1.0);
	}
}
