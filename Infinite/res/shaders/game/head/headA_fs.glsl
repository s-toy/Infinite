#define SHOW_DEBUG_GRID 0
#define DO_POSTPROCESS  1

#define _A 1
#define _B 2
#define _C 3
#define _D 4
#define _E 5
#define _F 6
#define _G 7
#define _H 8
#define _I 9
#define _J 10
#define _K 11
#define _L 12
#define _M 13
#define _N 14
#define _O 15
#define _P 16
#define _Q 17
#define _R 18
#define _S 19
#define _T 20
#define _U 21
#define _V 22
#define _W 23
#define _X 24
#define _Y 25
#define _Z 26

// System font (vec2 per character)
highp vec4 fnt0 = vec4(0, 0, 544926, 544959);
highp vec4 fnt1 = vec4(512159, 512161, 20638, 495745);
highp vec4 fnt2 = vec4(542863, 247969, 508095, 1032321);
highp vec4 fnt3 = vec4(508095, 16513, 20638, 495801);
highp vec4 fnt4 = vec4(1036449, 544929, 132158, 1016840);
highp vec4 fnt5 = vec4(528416, 495777, 115857, 542857);
highp vec4 fnt6 = vec4(16513, 1032321, 743841, 544929);
highp vec4 fnt7 = vec4(610721, 546985, 544926, 495777);
highp vec4 fnt8 = vec4(544927, 16543, 544926, 496805);
highp vec4 fnt9 = vec4(544927, 542879, 491678, 495776);
highp vec4 fnt10 = vec4(132223, 132104, 544929, 495777);
highp vec4 fnt11 = vec4(544929, 198945, 544929, 300705);
highp vec4 fnt12 = vec4(198945, 542988, 332097, 132104);
highp vec4 fnt13 = vec4(133183, 1032452, 132364, 1016840);
highp vec4 fnt14 = vec4(528542, 1032350, 397470, 495776);
highp vec4 fnt15 = vec4(165384, 139145, 508095, 495776);
highp vec4 fnt16 = vec4(508062, 495777, 266303, 66056);
highp vec4 fnt17 = vec4(495774, 495777, 544926, 495678);
highp vec4 fnt18 = vec4(678046, 496037, 0, 198144);
highp vec4 fnt19 = vec4(0, 66568, 0, 62);

vec2 fetchChrBitmap(float fi)
{
	int i = int(fi);
	int j = i / 2;
	int k = i - j * 2;

	vec4 b = fnt0;
#define F(x,y) b = j == x ? y : b;
	F(1, fnt1)F(2, fnt2)F(3, fnt3)F(4, fnt4)F(5, fnt5)F(6, fnt6)F(7, fnt7)F(8, fnt8)
		F(9, fnt9)F(10, fnt10)F(11, fnt11)F(12, fnt12)F(13, fnt13)F(14, fnt14)F(15, fnt15)
		F(16, fnt16)F(17, fnt17)F(18, fnt18)F(19, fnt19);
#undef F

	return (k == 0 ? b.xy : b.zw) + 0.5;
}

bool fetchBit(vec2 bitmap, float x, float y)
{
	float bits = y < 3.0 ? bitmap.y : bitmap.x;
	float index = float(x + 7.0 * (2.0 - (y < 3.0 ? y : y - 3.0)));
	float v = floor(bits * exp2(-index));
	v = x > 6.0 || y > 5.0 || x < 0.0 || y < 0.0 ? 0.0 : v;

	return floor(v * 0.5) * 2.0 != v;
}

bool fetchBoldBit(vec2 bitmap, float x, float y)
{
	bool bit = fetchBit(bitmap, x, y);

	bit = bit || fetchBit(bitmap, x - 1.0, y);

	if (y < 4.0 && x < 6.0)
	{
		bit = bit || fetchBit(bitmap, x - 2.0, y);
	}

	return bit;
}

vec3 getColor(int idx)
{
	vec3 col = vec3(0.0, 0.0, 0.0);

	idx = idx - (idx / 16) * 16;
	int i = idx - (idx / 8) * 8;

	col = i == 1 ? vec3(0, 0, 1) : col;
	col = i == 2 ? vec3(1, 0, 0) : col;
	col = i == 3 ? vec3(1, 0, 1) : col;
	col = i == 4 ? vec3(0, 1, 0) : col;
	col = i == 5 ? vec3(0, 1, 1) : col;
	col = i == 6 ? vec3(1, 1, 0) : col;
	col = i == 7 ? vec3(1, 1, 1) : col;

	col *= idx >= 8 ? 1.0 : 0.63;

	return col;
}

vec2 hash22(vec2 p)
{
	p = fract(p * vec2(5.3983, 5.4427));
	p += dot(vec2(p.y, p.x), p + vec2(21.5351, 14.3137));
	return fract(vec2(p.x * p.y * 95.4337, p.x * p.y * 97.597));
}

vec2 noise(float t)
{
	return hash22(vec2(t, t * 1.423)) * 2.0 - 1.0;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	float t = iTime;
	vec2 uv = .8 * (fragCoord - 0.5 * iResolution.xy) / iResolution.y;
	vec3 col = vec3(0.0);

	int border = 0;
	int scr = 0;
	int bkg = 0;
	int attr = 0;

	bool bold_font = true;
	bool show_loader = 1.0 < t;
	bool flash_phase = fract(t * 1.55) > 0.5;

	float at = floor(t * 11.1 + 0.35);

	vec2 o = vec2(16.0, 12.0);
	vec2 p = floor(uv * 32.0) + o;
	vec2 q = floor(uv * 32.0 * 8.0) / 8.0 + o;
	vec2 oq = q;

	vec3 c = vec3(0.0);

#define X(sym) h = floor(q.x)==cx ? vec3(sym,q.xy) : h; cx += 1.0;

	float cx = 0.0;

	// SGAME
    {
        bool hit = q.y>=21.5 && q.y<23.5;
        vec2 q = floor(uv * 16.0 * 8.0 + vec2(-4.0, 2.0)) / 8.0 + o;
        vec3 h = vec3(0.0);

        cx = 13.0;
		X(_S) X(_G) X(_A) X(_M) X(_E);

        int _attr = int(mod(-at, 8.0));
        c = hit ? h : c;
        attr = hit ? _attr : attr;
    }

	// INFINITE
	{
		bool hit = q.y>16.0 && q.y<20.0;
		vec2 q = floor(uv * vec2(12.0, 8.0) * 8.0) / 8.0 + o;
		vec3 h = vec3(0.0);

		cx = 12.0;
		X(_I) X(_N) X(_F) X(_I) X(_N) X(_I) X(_T) X(_E);

		int _attr = int(mod(abs(floor(oq.y) - 15.5) - at, 8.0));
		c = hit ? h : c;
		attr = hit ? _attr : attr;
	}

	// MADE BY IKUTO
	{
		bool hit = floor(q.y)==14.0;
		vec3 h = vec3(0.0);

		cx = 10.0;
		X(_M) X(_A) X(_D) X(_E);
		cx += 1.0;
		X(_B) X(_Y);
		cx += 1.0;
		X(_I) X(_K) X(_U) X(_T) X(_O);

		int _attr = int(mod(abs(floor(q.x) - 15.5) - at + 3.0, 8.0));
		c = hit ? h : c;
		attr = hit ? _attr : attr;
	}

	//PRESS SPACE TO START
	{
		bool hit = floor(q.y)==4.0;
		vec3 h = vec3(0.0);

		cx = 6.0;
		X(_P) X(_R) X(_E) X(_S) X(_S);
		cx += 1.0;
		X(_S) X(_P) X(_A) X(_C) X(_E);
		cx += 1.0;
		X(_T) X(_O)
		cx += 1.0;
		X(_S) X(_T) X(_A) X(_R) X(_T);

		c = hit ? h : c;
		attr = hit ? 7 : attr;
		bold_font = !hit;
	}

	attr = floor(q.y) == 12.0 ? int(mod(floor(q.x) + at + 3.0, 8.0)) : attr;
	attr = floor(q.y) == 13.0 ? int(mod(floor(q.x) - at + 3.0, 8.0)) : attr;
	attr = floor(q.y) == 14.0 ? int(mod(floor(q.x) + at, 8.0)) : attr;

	if (show_loader && q.y >= 8.0 && q.y < 12.0)
	{
		float xt = floor(at * 0.35);
		float fxt = fract(at * 0.35);

		float px = floor(q.x * 2.0);

		float fc = noise(px + xt * 32.0).x * 0.5 + 0.5;
		float fl = noise(px - 1.0 + xt * 32.0).x * 0.5 + 0.5;
		float fr = noise(px + 1.0 + xt * 32.0).x * 0.5 + 0.5;
		float fc2 = noise(px + (xt + 1.0) * 32.0).x * 0.5 + 0.5;
		float fl2 = noise(px - 1.0 + (xt + 1.0) * 32.0).x * 0.5 + 0.5;
		float fr2 = noise(px + 1.0 + (xt + 1.0) * 32.0).x * 0.5 + 0.5;

		fc = mix(fc, fc2, fxt);
		fl = mix(fl, fl2, fxt);
		fr = mix(fr, fr2, fxt);

		fc = sin(fc * 1.5) * 3.0 - 1.8;
		fl = sin(fl * 1.5) * 3.0 - 1.8;
		fr = sin(fr * 1.5) * 3.0 - 1.8;

		float y = (q.y - 8.0) / (12.0 - 8.0);
		float f = (2.0 * fc + fl + fr) * 0.25;
		y = floor(y * 8.0) / 8.0;

		bool mask = fract(q.x * 2.0 + 0.25) > 0.125;
		mask = mask && fract(q.y * 2.0 + 0.125) >= 0.25;

		if (f > y && mask) scr = 1;

		attr = int(mod(abs(floor(oq.y) - 15.5) + at * 1.0 + 3.0, 8.0));
	}

	vec2 bitmap = fetchChrBitmap(c.x);

	if (c.x > 0.0)
	{
		vec2 pix = fract(c.yz + vec2(0.0, -1.0 / 8.0)) * 8.0;
		if (bold_font)
			scr = int(fetchBoldBit(bitmap, pix.x, pix.y));
		else
			scr = int(fetchBit(bitmap, pix.x, pix.y));
	}

	// memory check
	if (t < 0.65)
	{
		scr = 0;
		attr = 2;
		bkg = 0;
		border = 7;

		float rt = t * 14.0 - 2.0;
		if (rt > 1.4) rt = 1.4 - (rt - 1.4) * 0.5;
		rt *= 24.0;

		float ry = mod(mod(q.y, 8.0) * 8.0, 8.0) + floor(q.y / 8.0) * 8.0;

		if (fract(q.x) * 8.0 == 6.0 && ry < rt) scr = 1;
	}

	col = getColor(scr > 0 ? attr : bkg);

#if SHOW_DEBUG_GRID
	col += fract(floor(q.y) * 0.5 + floor(q.x) * 0.5) > 0.0 ? 0.2 : 0.0;
#endif

	if (p.x < 0.0 || p.x >= 32.0 || p.y < 0.0 || p.y >= 24.0) col = getColor(border);

#if DO_POSTPROCESS
	{
		col *= col;
		vec2 sp = (fragCoord.xy + 0.000) * 0.15 * 3.1415 * 4.0;
		vec3 m = vec3(
			1.0 - pow(1.0 - (sin(sp.x - 0.0) * 0.1 + 0.9), 0.75),
			1.0 - pow(1.0 - (sin(sp.x - 2.0) * 0.1 + 0.9), 0.75),
			1.0 - pow(1.0 - (sin(sp.x - 3.0) * 0.1 + 0.9), 0.75)
		) * (1.0 - pow(1.0 - (sin(sp.y * 0.9 - 0.8) * 0.2 + 0.8), 2.0));

		vec3 em = vec3(0.04, 0.3, 0.08);
		em *= pow(max(0.0, 1.5 - 1.2 * dot(uv, uv)), 2.0) * 0.008;
		col += em * m;

		col = col * (0.4 + 0.7 * m);

		col *= 1.0 + 0.015 * sin(uv.y * 4.0 + t * 33.0);
		col *= 1.2 - 0.4 * dot(uv, uv);

		float g = 0.38;
		col.x = pow(col.x, g);
		col.y = pow(col.y, g);
		col.z = pow(col.z, g);
		col = mix(col, smoothstep(0.0, 1.0, col), 0.5);
	}
#endif

	fragColor = vec4(col, 1.0);
}