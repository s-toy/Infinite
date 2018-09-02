#version 430 core

uniform vec2 iResolution;
uniform float iRadius;

in vec2 _TexCoords;

out vec4 _outFragColor;

vec4 circle(vec2 fragCoord, vec2 centerPos, float radius, vec3 color)
{
	float d = length(fragCoord - centerPos) - radius;
	float t = smoothstep(0.0, 2.0, d);
	return vec4(color, 1.0 - t);
}

void main()
{
	vec2 fragCoord = _TexCoords * iResolution;
	vec4 circleCol = circle(fragCoord, 0.5 * iResolution.xy, iRadius, vec3(1.0, 1.0, 1.0));
	_outFragColor = circleCol;

	if (_outFragColor.a < 0.01) discard;
}