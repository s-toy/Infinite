const int KEY_SPACE = 32;
const int KEY_LEFT  = 37;
const int KEY_UP    = 38;
const int KEY_RIGHT = 39;
const int KEY_DOWN  = 40;
const int KEY_A     = 65;
const int KEY_D     = 68;
const int KEY_S     = 83;
const int KEY_W     = 87;

bool KP(int key) {
	return texelFetch( iChannel0, ivec2(key, 0), 0 ).x > 0.0;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord/iResolution.xy;

	if (KP(KEY_D))
		fragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if (KP(KEY_SPACE))
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragColor = texture(iChannel0, uv);
}