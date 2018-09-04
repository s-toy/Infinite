void mainImage(out vec4 fragColor, vec2 fragCoord)
{
	vec2 uv = fragCoord / iResolution;
	if (uv.x < 0.5 && uv.y < 0.5)
		fragColor = texture(iChannel0, 2. * uv);
	else if (uv.x < 0.5 && uv.y >= 0.5)
		fragColor = texture(iChannel1, 2. * vec2(uv.x, uv.y - .5));
	else if (uv.x >= 0.5 && uv.y < 0.5)
		fragColor = texture(iChannel2, 2. * vec2(uv.x - .5, uv.y));
	else
		fragColor = texture(iChannel3, 2. * vec2(uv.x - .5, uv.y - .5));
}