#version 330
//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; 

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(void) 
{
gl_Position =  projectionTransform * viewTransfrom * modelTransform * vec4 (in_Position, 1.0) ;
}