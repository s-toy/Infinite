#define N(h) fract(sin(vec4(6,9,1,0)*h) * 9e2) 
void mainImage(out vec4 o,vec2 u )
{
    o-=o; 
    
    u /= iResolution.y;
    
    float e, d, i=-2.; 
    
    for(vec4 p; i++<9.; d = floor(e = i*9.1+iTime),p = N(d)+.3, e -= d)
        for(d=0.; d++<50.;)
            o += p*(1.-e) / 1e3 / length(u-(p-e*(N(d*i)-.5)).xy);  

    if(u.y<N(ceil(u.x*i+d+e)).x*.4) o-=o*u.y;
}