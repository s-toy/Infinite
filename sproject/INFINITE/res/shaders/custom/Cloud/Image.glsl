//---------------------------------------------------------------------------
//2D Perlin noise implementation 
//---------------------------------------------------------------------------
#define HASHSCALE 0.1031

float hash(vec2 p)
{
	vec3 p3  = fract(vec3(p.xyx) * HASHSCALE);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

vec2 fade(vec2 t) { return t*t*t*(t*(6.*t-15.)+10.); }

float grad(float hash, vec2 p)
{
    int i = int(1e4*hash);
	return ((i & 1) == 0 ? p.x : -p.x) + ((i & 2) == 0 ? p.y : -p.y);
}

float perlinNoise2D(vec2 p)
{
	vec2 pi = floor(p), pf = p - pi, w = fade(pf);
    return mix( mix( grad(hash(pi + vec2(0, 0)), pf - vec2(0, 0)),
                      grad(hash(pi + vec2(1, 0)), pf - vec2(1, 0)), w.x ),
            	 mix( grad(hash(pi + vec2(0, 1)), pf - vec2(0, 1)), 
                	  grad(hash(pi + vec2(1, 1)), pf - vec2(1, 1)), w.x ), w.y );
}

float fbm(vec2 pos, int octaves, float persistence) 
{
    float total = 0., frequency = 1., amplitude = 1., maxValue = 0.;
    for(int i = 0; i < octaves; ++i) 
    {
        total += perlinNoise2D(pos * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.;
    }
    return total / maxValue;
}

//---------------------------------------------------------------------------
//Rendering: four simple demos
//upper left:   1d perlin noise curve
//upper right:  simple 2D terrain
//lower left:   hand drawn effect
//lower right:  2D water surface
//---------------------------------------------------------------------------
#define FADE_SCALE 30.0
#define MAX_DISTANCE 10.
#define NUM_OCTAVES 6

float getNoise(vec3 p)
{
    vec2 uv = (p.xy+ceil(p.z)) ;
    return perlinNoise2D(uv);
}

vec4 renderUL(in vec2 uv)
{
	vec3 ray = vec3(uv.x, uv.y - .3, 1.);
   
    vec3 skyCol = vec3(.3, .5, .8);
    vec3 sunDir = normalize(vec3(.0, .03, 1.));
    vec3 col = skyCol - ray.y;
    col += vec3(1., .7, .5) * pow(max(dot(normalize(ray), sunDir), .0), 50.) * .7; 
    
    float offset = .05*sin(dot(uv, uv));
    for (float d = MAX_DISTANCE + offset; d > 0.; d-=.05)
    {
        vec3 pos = d * ray;
        pos.xz += iTime;
        
        //float noise = getNoise(pos);
        float noise = 0., frequency = 1., amplitude = 1.;
        for (int k = 0; k < NUM_OCTAVES; ++k)
        {
        	noise += getNoise(pos * frequency) * amplitude;     
            frequency *= 2.;
            amplitude *= .5;
        }
        
        float dist = noise + pos.y + .5;
        if (dist < 0.) 
            col = mix(col, 1.+dist*skyCol.zyx, -dist*.4);
	}
    
    return vec4(col, 1.);
}

vec4 renderUR(in vec2 uv)
{
	return vec4(0.0);
}

vec4 renderLL(in vec2 uv)
{
	return vec4(0.0);
}

vec4 renderLR(in vec2 uv)
{
	return vec4(0.0);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    float aspect = iResolution.x / iResolution.y;
    vec2 uv = (2.*fragCoord-iResolution.xy) / iResolution.y;
    
    fragColor = renderUL(uv);
    

}