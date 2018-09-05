// Some value noise that only gets rendered for the first 5 frames,
// or when the resolution has changed. In order to detect the resolution change
// the width of the screen is written into the alpha channel. 
// I took this idea from one of Shane's shaders.
// Its used to give some texture to the background. I tried using it to texture the
// body and the womb walls but it just turned out to be distracting and it was too hard to
// get proper uv coordinates that would stick on the body while its kicking so in the end
// I decided that less is more ;)


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy/iResolution.xy;
    vec4 col = texture(iChannel0, uv);
    if(iFrame<5|| abs(iResolution.x - col.a)>.001) {
    	vec3 center = LayerNoise3(uv*5.);
        vec3 edge = LayerNoise3(fract(uv-vec2(.5))*5.);
        
        uv -= .5;
        col.rgb = mix(center, edge, dot(uv, uv));
    }

	col.a = iResolution.x;
    
    fragColor = col;
}