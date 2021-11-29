#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec2 TexCoord;
void main()
{
gl_Position = projectionTransform * viewTransform * modelTransform * vec4(in_Position, 1.0);
TexCoord = vTexCoord;
}