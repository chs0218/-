#version 330

in vec3 out_Color;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main(void) 
{
	float ambientLight = 0.3;
	vec3 ambient = ambientLight * vec3(1.0, 1.0, 1.0);
	
	vec3 normalVector = normalize (Normal);
	vec3 lightDir = normalize (lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;
	
	vec3 result = (ambient + diffuse) * out_Color;
	FragColor = vec4 (result, 1.0);
}
