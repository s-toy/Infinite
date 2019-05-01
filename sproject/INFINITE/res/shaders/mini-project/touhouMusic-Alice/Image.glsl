void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	vec4 layer0 = texture(iChannel0, uv); //background && bubble
	vec4 layer1 = texture(iChannel1, uv); //waveform
	vec4 outColor = mix(layer0, layer1, layer1.a);

	fragColor = outColor;
}