//the maze code is from Basic Maze Generator by stb https://www.shadertoy.com/view/XdKGWK

#define LONG_CORRIDORS
#define ADD_ROOMS

#define PI 3.14159265
#define KEY_UP 265
#define KEY_DOWN 264
#define KEY_LEFT 263
#define KEY_RIGHT 262
#define KEY_R 82

bool KeyDown(in int key){
	return (texture(iChannel1,vec2((float(key)+0.5)/512.0, 0.25)).x>0.0);
}

const vec2 MazeSize = 4. * vec2(16., 9.);

// hash without sine
// https://www.shadertoy.com/view/4djSRW
#define MOD3 vec3(443.8975,397.2973, 491.1871)
float hash12(vec2 p) {
	vec3 p3  = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}
#define rotateTo(p, a) mat2(a.y, a.x, -a.x, a.y) * p
vec2 tx_cos(vec2 a){return abs(mod(a,4.0)-2.0)-1.0;}
vec2 tx_cossin(float a){return tx_cos(vec2(a,a-1.0));}

float drawWall(in vec2 p, vec2 dr) {
    p = fract(rotateTo(p, dr)) - .5;
    p.y = max(0., -p.y);
    return length(p) - 0.15;
}

#define t2D(o) textureLod(iChannel0, (o)/iResolution.xy, 0.0)
#define eq(a,b) (abs(a-b)<0.01)
float dot2(vec2 a){return dot(a,a);}
float sgn(float x){return (x<0.0?-1.:1.);}
#define eq2(a,b) (dot2(a-(b))<0.01)

float DE(vec2 p){
	vec2 fp=floor(p)+vec2(0.5);
	float wall=drawWall(p, t2D(fp).rg);
	for(float i=0.0; i<4.0; i++){
		vec2 dr=tx_cossin(i),dr2=t2D(fp-dr).rg;
       	if eq2(dr, dr2) wall = min(wall, drawWall(p, -dr2));
	}
	return wall;
}
void GoGoGo(inout vec4 st1, inout vec4 st2, float dt, float fw){
	vec3 dr=vec3(-sin(st2.x),cos(st2.x),0.0)*fw;
	vec2 p=st1.xz+dr.xy*0.1;
	float d=DE(p);
	if(d>0.15){
		st1.xz+=dr.xy*min(d,dt*2.0);
		st2.y+=dt*10.0;
	}else{//Dave's sliding trick works well here
		p=st1.xz+dr.xz*0.1;
		d=DE(p);
		if(d>0.15){
			st1.xz+=dr.xz*min(d,dt*2.0);
			st2.y+=dt*10.0;
		}else{
			p=st1.xz+dr.zy*0.1;
			d=DE(p);
			if(d>0.15){
				st1.xz+=dr.zy*min(d,dt*2.0);
				st2.y+=dt*10.0;
			}
		}
	}
}
void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec4 st2=texture(iChannel0,vec2(1.5,100.5)/iResolution.xy);
	if(floor(fragCoord.y)==100.0 && fragCoord.x<2.0){//walking
		vec4 st1;
		if(iFrame==0 || KeyDown(KEY_R)){
			st1=vec4(1.1,-0.4,1.1,iTime);//pos,time
			st2=vec4(0.0,0.0,0.0,0.0);//dir,anim,frame,cam dir
		}else{
			st1=texture(iChannel0,vec2(0.5,100.5)/iResolution.xy);
		}
		float dt=iTime-st1.w;
		
		if(KeyDown(KEY_UP))GoGoGo(st1,st2,dt,1.0);
		if(KeyDown(KEY_DOWN))GoGoGo(st1,st2,dt,-1.0);
		if(KeyDown(KEY_LEFT))st2.x+=dt*2.0;
		if(KeyDown(KEY_RIGHT))st2.x-=dt*2.0;
        if(iMouse.z>0.0)st2.x-=(iMouse.x-iMouse.z)*dt*0.02;

        st2.z+=1.;
		st2.w+=(st2.x-st2.w)*min(dt*10.0,1.0);
		st1.w=iTime;
		fragColor=(fragCoord.x<1.0?st1:st2);
		return;
	}
	if(fragCoord.y>MazeSize.y || fragCoord.x>MazeSize.x){fragColor=vec4(0.0);return;}
	
    //maze by stb
	
    vec2 fg = floor(fragCoord);

    // saving a few chars
    vec3 o = vec3(1., -1., 0.);
    
    // wall direction (will be stored)
    vec2 wdir = o.zz;
    
    // initialize wall boundaries
    if(iFrame==0 || KeyDown(KEY_R)) {
        if(fg.y==0.)
            wdir = o.xz;
        if(fg.y==MazeSize.y-1.)
            wdir = o.yz;
        if(fg.x==0.)
            wdir = o.zx;
        if(fg.x==MazeSize.x-1.)
            wdir = o.zy;
        if(fg.x==MazeSize.x/2. && abs(fg.y-MazeSize.y/2.0)<MazeSize.y/4.0)
            wdir = o.zx;
    }
    // create maze walls
    else {
	fg+=vec2(0.5);
        // this cell's stored wall direction, if any
        wdir = t2D(fg).rg;
        
        // is this cell empty?
        if(st2.z<1000. && dot2(wdir)<0.01)
#ifdef ADD_ROOMS
            if(length(abs(fg-0.5*MazeSize)-0.25*MazeSize)>3.0) 
#endif
            {
            
            // a random, cardinal direction
			float a=floor(4.0*hash12(fg+mod(iDate.w/100., 100.)));
			vec2 r=tx_cossin(a);//i was trying diagonal walls... nope (eiffie)

			// if wall exists in direction r, make a new wall using that direction
            if(dot2(t2D(fg+r).rg)>0.01)
                //if(.5>dot(r, t2D(fg+r).rg))
#ifdef LONG_CORRIDORS
				if(dot(r, t2D(fg+r).rg)>.5 || .9<hash12(-fg-mod(iTime/100., 100.)))
#endif
					wdir = r;
        }
    }
    
    // store wall direction
    fragColor = vec4(wdir,0.0,0.0);

}