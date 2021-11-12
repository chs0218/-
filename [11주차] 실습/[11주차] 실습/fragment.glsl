#version 330

in vec3 out_Color;

uniform vec3 lightColor;

out vec4 FragColor;

void main(void) 
{
	float ambientLight = 0.5;
	vec3 ambient = ambientLight * lightColor;
	vec3 result = ambient * out_Color;

	FragColor = vec4 (result, 1.0);
	FragColor = vec4 (out_Color, 1.0);
}
