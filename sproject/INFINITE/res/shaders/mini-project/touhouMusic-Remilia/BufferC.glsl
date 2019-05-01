#define PI 3.1415926

#define HEIGHT 0.02
#define BARS_NUM 128.0
#define BAR_SIZE 1.0 / BARS_NUM
#define BAR_GAP 0.1 * BAR_SIZE      // 0.1 represents gap on both sides, so a bar is
#define SAMPLE_SIZE 0.02            // How accurately to sample spectrum, must be a factor of 1.0

//-----------------------------------------------------------------------------------------------------------------------------------
//2D distance functions from iq's article: http://www.iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
float sdLine(in vec2 p, in vec2 a, in vec2 b)
{
	vec2 pa = p - a, ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
	return length(pa - ba * h);
}

float sdCircle(vec2 p, float r)
{
	return length(p) - r;
}

vec4 annularSector(vec2 uv, vec2 center, float minRad, float maxRad, int divideNum, vec3 color)
{
	vec2 v = normalize(uv - center);
	float theta = asin(v.y);
	theta = v.x > 0 ? theta : (-theta + PI);
	theta += iTime;

	float delta = 2.0*PI / divideNum;
	theta = mod(theta, delta);
	if (theta < delta * 0.2) return vec4(0.0);

	float d = length(uv - center);
	d = max(d - maxRad, minRad - d);

	float t = (d < 0 ? 1.0 : 0.1*minRad / d);
	return vec4(color, clamp(t, 0.0, 1.0));
}

//based on: https://www.shadertoy.com/view/XdX3z2
vec4 waveform(vec2 uv) {
	if (any(bvec4(greaterThan(uv, vec2(1.0)), lessThan(uv, vec2(0.0))))) return vec4(0.0);

	float barStart = floor(uv.x * BARS_NUM) / BARS_NUM;
	if (uv.x - barStart < BAR_GAP || uv.x > barStart + BAR_SIZE - BAR_GAP) {
		return vec4(0.0);
	}
	else {
		float intensity = 0.0;
		for (float s = 0.0; s < BAR_SIZE; s += BAR_SIZE * SAMPLE_SIZE)
			intensity += texture(iChannel0, vec2(barStart + s, 0.25)).r;
		intensity *= SAMPLE_SIZE;
		intensity = clamp(intensity, 0.005, 1.0);
		float i = float(intensity > uv.y);
		return vec4(vec3(0.9, 0.0, 0.2) * i, clamp(i*uv.y*30.0, 0.0, 0.6));
	}
}

vec4 renderPlaybackBar(vec2 uv)
{
	float playbackPos = 1000.0 * iTime / iAudioDuration;

	vec3 baseLineCol = vec3(0.8, 0.0, 0.0);
	float alpha = 0.001 / sdLine(uv, vec2(0.0, HEIGHT), vec2(1.0, HEIGHT));
	vec4 lineCol = vec4(baseLineCol, alpha);

	float sd = sdCircle(uv - vec2(playbackPos, HEIGHT), 0.002);
	alpha = sd < 0 ? 1.0 : 0.002 / sd;
	vec4 circleCol = vec4(vec3(1.0), clamp(alpha, 0.0, 1.0));

	vec4 sectorCol = 2.0 * annularSector(uv, vec2(playbackPos, HEIGHT), 0.01 + 0.002*sin(iTime), 0.012 + 0.002*sin(iTime), 20, vec3(0.6));
	sectorCol.a *= 0.2;

	float minY = HEIGHT, maxY = 0.1;
	vec4 waveCol = waveform(vec2(uv.x, (uv.y > minY && uv.y < maxY) ? (uv.y - minY) / (maxY - minY) : -1.0));

	vec4 col = lineCol;
	col = mix(col, waveCol, waveCol.a);
	col = mix(col, circleCol, circleCol.a);
	col = mix(col, sectorCol, sectorCol.a);
	return col;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord / iResolution.x;
	fragColor = renderPlaybackBar(uv);
}