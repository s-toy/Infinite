#version 430 core

uniform vec2 iResolution;

layout (location = 0) in vec3 _inFragPosition;
layout (location = 1) in vec2 _inTexCoords;

out vec2 fragCoord;

void main()
{
	gl_Position = vec4(_inFragPosition, 1.0f);
	fragCoord = _inTexCoords * iResolution;
}