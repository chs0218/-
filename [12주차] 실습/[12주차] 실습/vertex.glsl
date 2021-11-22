#version 330
//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; 
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec3 vNormal;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 out_Color;
out vec3 FragPos;
out vec3 Normal;

void main(void) 
{
gl_Position =  projectionTransform * viewTransform * modelTransform * vec4 (in_Position, 1.0) ;
FragPos = vec3(modelTransform * vec4(in_Position, 1.0));
Normal = vec3(modelTransform * vec4(vNormal, 1.0));
out_Color = in_Color;
}