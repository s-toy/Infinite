void mainImage(out vec4 o, vec2 u)
{
	o = 1.-texelFetch(iChannel0, ivec2(u) ,0);
}