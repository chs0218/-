#version 330

in vec3 out_Color;
in vec3 Normal;

out vec4 FragColor;

void main(void) 
{
	FragColor = vec4 (out_Color, 1.0);
}
