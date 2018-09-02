void mainImage(out vec4 fragColor, vec2 fragCoord)
{
	vec2 uv = (fragCoord - .5*iResolution.xy) / iResolution.y;
	vec3 ray = vec3(uv.x, uv.y - .3, 1.);
	vec3 sunDir = normalize(vec3(.0, .0, 1.));

	vec3 col = vec3(.3, .5, .8) - ray.y;
	col += vec3(1., .7, .5) * pow(max(dot(normalize(ray), sunDir), .0), 50.) * .9;

	fragColor = vec4(col, 1.);
}