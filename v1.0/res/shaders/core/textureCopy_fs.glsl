#version 430 core

uniform sampler2D uTexture;

in vec2 _TexCoords;

out vec4 _outFragColor;

void main()
{
	_outFragColor = texture(uTexture, _TexCoords);
}