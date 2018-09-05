#define S(a,b,t) smoothstep(a,b,t)

// http://iquilezles.org/www/articles/smin/smin.htm
float smin( float a, float b, float k ) {
    float h = clamp( 0.5+0.5*(b-a)/k, 0., 1. );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float N2(vec2 p) {	// Dave Hoskins - https://www.shadertoy.com/view/4djSRW
	vec3 p3  = fract(vec3(p.xyx) * vec3(443.897, 441.423, 437.195));
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}
float N2(float x, float y) { return N2(vec2(x, y)); }

vec3 N23(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+19.19);
    return fract((p3.xxy+p3.yzz)*p3.zyx);
}
vec3 N23(float x, float y) {return N23(vec2(x, y));}



float SmoothNoise(vec2 uv) {
    // noise function I came up with
    // ... doesn't look exactly the same as what i've seen elswhere
    // .. seems to work though :)
    vec2 id = floor(uv);
    vec2 m = fract(uv);
    m = 3.*m*m - 2.*m*m*m;
    
    float top = mix(N2(id.x, id.y), N2(id.x+1., id.y), m.x);
    float bot = mix(N2(id.x, id.y+1.), N2(id.x+1., id.y+1.), m.x);
    
    return mix(top, bot, m.y);
}

float LayerNoise(vec2 uv) {
    float c = SmoothNoise(uv*4.);
    c += SmoothNoise(uv*8.)*.5;
    c += SmoothNoise(uv*16.)*.25;
    c += SmoothNoise(uv*32.)*.125;
    c += SmoothNoise(uv*65.)*.0625;
    
    return c / 2.;
}

vec3 SmoothNoise3(vec2 uv) {
    // noise function I came up with
    // ... doesn't look exactly the same as what i've seen elswhere
    // .. seems to work though :)
    vec2 id = floor(uv);
    vec2 m = fract(uv);
    m = 3.*m*m - 2.*m*m*m;
    
    vec3 top = mix(N23(id.x, id.y), N23(id.x+1., id.y), m.x);
    vec3 bot = mix(N23(id.x, id.y+1.), N23(id.x+1., id.y+1.), m.x);
    
    return mix(top, bot, m.y);
}

vec3 LayerNoise3(vec2 uv) {
    vec3 c = SmoothNoise3(uv*4.);
    c += SmoothNoise3(uv*8.)*.5;
    c += SmoothNoise3(uv*16.)*.25;
    c += SmoothNoise3(uv*32.)*.125;
    c += SmoothNoise3(uv*65.)*.0625;
    
    return c / 2.;
}


/*
float dEllipsoid(vec3 p, vec3 a, vec3 b, float r) {
	float d1 = length(p-a);
    float d2 = length(p-b);
    float d3 = length(a-b);
    return mix(d1, d2, .5)-.5*d3-r;
}*/



