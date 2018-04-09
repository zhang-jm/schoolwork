#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 Color;

layout(location = 0) out vec4 color;

uniform int useNormals;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main()
{   
    if(useNormals == 1) {
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(vec3(0.0f, 150.0f, 100.0f) - FragPos);  
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);
		vec4 texColor = texture(texture_diffuse, TexCoords);
		vec3 result = diffuse * vec3(texColor.x, texColor.y, texColor.z);
		//color = vec4(Normal, 1.0f);
		color = vec4(result, 1.0f);
		//color = texture(texture_diffuse, TexCoords);
		//color = vec4(vec3(gl_FragCoord.z), 1.0);
	}
	else {
      color = texture(texture_diffuse, TexCoords);
	}
}
