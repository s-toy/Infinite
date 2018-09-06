#version 430 core

uniform float uKeyState[768];

in vec2 _TexCoords;

out float _outFragColor;

void main()
{
	vec2 uv = floor(_TexCoords * vec2(256.0, 3.0));
	_outFragColor = uKeyState[32*3];
}