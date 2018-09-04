#version 430 core

uniform mat4 uModelMatrix = mat4(1.0);

layout (location = 0) in vec3 _inFragPosition;
layout (location = 1) in vec2 _inTexCoords;

out vec2 _TexCoords;

void main()
{
	gl_Position = uModelMatrix * vec4(_inFragPosition, 1.0f);
	_TexCoords = _inTexCoords;
}