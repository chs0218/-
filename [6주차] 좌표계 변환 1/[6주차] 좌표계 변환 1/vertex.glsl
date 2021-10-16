#version 330
//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; 
layout (location = 1) in vec3 in_Color;

uniform mat4 modelTransform;

out vec3 out_Color;
void main(void) 
{
gl_Position = modelTransform * vec4 (in_Position, 1.0) ;
out_Color = in_Color;
}