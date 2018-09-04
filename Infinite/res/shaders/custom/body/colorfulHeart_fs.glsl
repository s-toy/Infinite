#define tau 6.2831853
#define grad(val) vec2(1.0 - (val), (val)) 

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
	vec2 p = (2.0 * fragCoord - iResolution.xy) / iResolution.y + vec2(0.0, -0.6);
	vec2 uv = vec2(atan(p.x, p.y) / tau, length(p));

	//get the color
	float u = fract(uv.x + (iTime * 0.2)) * 3.0;
	vec3 horColour = vec3(0.5);

	if (u < 1.0)
		horColour.rg += grad(u);
	else if (u < 2.0)
		horColour.gb += grad(u - 1.0);
	else
		horColour.br += grad(u - 2.0);

	// draw color beam
	float t = clamp(floor(5.0 + 10.0 * cos(iTime)), 0.0, 10.0);
	float horBeam = (0.7 + 0.5 * cos(30.0 * uv.x * t)) * abs(1.0 / (60.0 * (uv.y - 2.5 * abs(uv.x))));
	fragColor = vec4(horBeam * horColour, 1.0);
}