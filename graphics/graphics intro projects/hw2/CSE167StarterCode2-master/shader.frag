#version 330 core
// This is a sample fragment shader.
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	vec3 direction;
	vec3 position;
	
	float cutoff;
	float exponent;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform int normal;
uniform int light;
uniform float quad;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{      
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result; 

	if(light == 0) {	
		result = CalcDirLight(dirLight, norm, viewDir);
	}

	else if(light == 1) {
		result = CalcPointLight(pointLight, norm, FragPos, viewDir);
	}

	else{
		result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
	}

    if(normal == 0) {
		color = vec4(result, 1.0f);
	}

	else {
		color = vec4(norm.x, norm.y, norm.z, 1.0f); //set color using normalized values
	}
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
	// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (quad * (distance * distance));    
    
	// Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	// Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (quad * (distance * distance));
	    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.exponent;
    float intensity = clamp((theta - light.exponent) / epsilon, 0.0, 1.0);

	if(intensity < 1){
		if(intensity >= 0.0) {
			intensity = 1 - intensity;
		}
	} 

	//float intensity = pow(theta, light.exponent);

    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

	if(theta < light.cutoff) {
		return ambient;
	}
	else{
	 	return (ambient + diffuse + specular);
	}
}
