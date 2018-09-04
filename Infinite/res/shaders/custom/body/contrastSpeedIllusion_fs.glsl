void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord.xy / iResolution.xy;

	float fade = smoothstep(0.1, 0.2, abs(fract(iTime * 0.2 + 0.5) - 0.5));
	vec3 bgpattern = vec3(round(fract(uv.x * 40.0)));

	float x = step(abs(uv.x - fract(iTime * 0.08)), 0.05);
	float y1 = step(abs(uv.y - 0.3), 0.03);
	float y2 = step(abs(uv.y - 0.7), 0.03);

	vec3 col = mix(vec3(0.0), bgpattern, fade);
	col = mix(col, vec3(1, 1, 0), x * y1);
	col = mix(col, vec3(0, 0, 1), x * y2);

	fragColor = vec4(col, 1.0);
}