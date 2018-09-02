#version 430 core

uniform sampler2D uTexture;

uniform vec2 iResolution;

in vec2 fragCoord;

out vec4 fragColor;

void main()
{
	vec2 uv = fragCoord / iResolution;
    fragColor = texture(uTexture, uv);
}