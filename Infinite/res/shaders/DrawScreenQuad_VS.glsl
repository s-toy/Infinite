#version 430 core

layout (location = 0) in vec3 _inFragPosition;
layout (location = 1) in vec2 _inTexCoords;

out vec2 _TexCoords;

void main()
{
	gl_Position = vec4(_inFragPosition, 1.0f);
	_TexCoords = _inTexCoords;
}