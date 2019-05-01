#define IMAGE_NUM		4
#define LAST_TIME		10.0
#define SWITCH_DELAY	2.0

//TODO: 实现shader的复用
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord / iResolution.xy;
	float t = mod(iTime, LAST_TIME * IMAGE_NUM);

	int currChannelIndex = int(t / LAST_TIME);
	int nextChannelIndex = (currChannelIndex + 1 < IMAGE_NUM) ? currChannelIndex + 1 : 0;

	vec4 col = mix(
		texture(iChannel[currChannelIndex], uv),
		texture(iChannel[nextChannelIndex], uv),
		smoothstep(0.0, SWITCH_DELAY, t - (currChannelIndex + 1) * LAST_TIME + SWITCH_DELAY)
	);

	fragColor = col;
}