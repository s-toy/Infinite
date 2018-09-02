#version 430 core

uniform vec2 iResolution;

in vec2 fragCoord;

out vec4 fragColor;

vec4 circle(vec2 fragCoord, vec2 centerPos, float radius, vec3 color)
{
	float d = length(fragCoord - centerPos) - radius;
    float t = smoothstep(0.0, 2.0, d);
    return vec4(color, 1.0 - t);
}

void main()
{    
    vec4 circleCol = circle(fragCoord, 0.5 * iResolution.xy, 0.4 * iResolution.y, vec3(1.0, 1.0, 1.0));
    fragColor = circleCol;
    
	if (fragColor.a < 0.01) discard;
}