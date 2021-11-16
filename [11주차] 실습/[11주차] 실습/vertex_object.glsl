#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 vNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 rotateTransform;
uniform mat4 projectionTransform;

void main()
{
gl_Position = projectionTransform * viewTransform * modelTransform * vec4(in_Position, 1.0);
FragPos = vec3(modelTransform * vec4(in_Position, 1.0));
Normal = vec3(rotateTransform * vec4(vNormal, 1.0));
}