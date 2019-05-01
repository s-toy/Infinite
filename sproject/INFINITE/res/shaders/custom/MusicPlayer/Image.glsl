/********************************************
    Weight Of The World.glsl webgl2.0 Created by 834144373
    2018.1.1
	PS：834144373 is TNWX or 恬纳微晰 or 祝元洪
*********************************************/
/*
	This Cover Texture Compression technology is used by ATC (ATITC) 
	And it's 4bpp!
	1. BufA and BufB is fast store full 32bit float into 16bit half-float buffer,
	2. BufC is ATC Decoder,
	3. Image is 834144373's Audio Player.

If you want to know more about GPU Texture Compression,
		the below Texture Compression Can help you.:)
------------------------------------------------------------------------
DXT1           4bpp : https://www.shadertoy.com/view/MtBfRR by 834144373
BC4            4bpp : https://www.shadertoy.com/view/Xlffz2 by 834144373
ShaderToyTC I  1bpp : https://www.shadertoy.com/view/MtyGzV by 834144373
PVRTC          4bpp : https://www.shadertoy.com/view/ltV3zD by 834144373
YUV Special    2bpp : https://www.shadertoy.com/view/XlGGzh by 834144373
...more Confidentiality Texture Compression will be coming soon and GIF!!!
------------------------------------------------------------------------

Node : Yes! You didn't read the wrong words!
       more Confidentiality Texture Compression will be coming soon!!!
	   Crack Crack and Crack!!!
*/
//---------------------------Change-----------------------
#define Cover_Color_Version 2. //1.,2.or 3.  (I like version 2.)
//
#define R iResolution.xy
#define A_time iTime
#define d_ddxddy length(1./R*(R.x/R.y)*1.5) //Perfect Pixel Edge
#define PI 3.14159265354
float box(vec2 p,vec2 size){
	return length(max(abs(p)-size,0.));
}
float sphere(vec2 p){
	return length(p);
}
float line(vec2 p,vec2 a,vec2 b)
{
    vec2 pos = p - a;
    vec2 ab = b - a; // ->ab
    return length(pos-ab*clamp(dot(pos,ab)/dot(ab,ab),0.,1.));
}
//fast-tiny Bezier2D https://www.shadertoy.com/view/MsKGDV from 834144373
#define M(k,t) (k*mat2(t.y,-t.x,0,0)).xx,
float bezier(vec2 p,vec2 A,vec2 B,vec2 C){
	A -= p,B -= p,C -= p;
    vec2 x = .5*M(C,A) y=M(B,C) z=M(A,B) D=B-C,
    u = x*(A-C)+y*(A-=B)+z*D;
    u.x = -u.x;
    A -= D;
    x = clamp(.5*M((C+(y*z-x*x)*u.yx/dot(u,u)),A)  x-x, y+=x+x+z ) / y;
    return length(C + x * (D+D + x * A));
}

float GetAlpha(float d){
	return smoothstep(d_ddxddy,0.,d);
}
float Cover_Size = 600.;
vec2 Cover_Offset = vec2(260.,340.);
float Cover_Radius = 128.;
float Texture_Size = 128.;
//
vec2 Rotation(vec2 uv,float time){
	mat2 rot = mat2(cos(time-.78*vec4(0,6,2,0)));
    return uv*rot;
}
vec3 GetBackGround(vec2 uv){
    //Fast Blur With Mipmap For BackGround
	vec3 blur_tex = texture(iChannel0,uv/(R/Texture_Size),4.2).rgb;
     for(float i = 0.;i<12. ;i++){
		  blur_tex += texture(iChannel0,uv/(R/Texture_Size)-i/R,mod(i,6.)+6.).rgb;
    }
    blur_tex /= 11.*1.3;
    return blur_tex;
}
vec3 GetCoverTex(vec2 uv){
	vec3 cover = texture(iChannel0,uv/R.xy/2./(R.y/Cover_Size)).rgb;
    cover *= 1.24;
    //color version 1
    if(Cover_Color_Version == 1.){
    	cover = pow(cover,vec3(1.15,1.2,1.1));
    	cover = pow(cover,vec3(1.15,1.1,1.3));
    }
    else if(Cover_Color_Version == 2.){
	    cover = pow(cover,vec3(1.5,1.3,1.1));
    }
    else if(Cover_Color_Version == 3.){
    	cover = pow(cover,vec3(1.2,1.3,1.13));
    }
    return cover;
}
vec3 GetCover(vec2 U){
	vec2 uv = U/R;    
    U -= Cover_Offset*(R.y/Cover_Size);
    U = Rotation(U,iTime*0.15);
	U += Cover_Radius*(R.y/Cover_Size);
    
    vec2 uv_ring = (U+U)/R.y-(Texture_Size*2./Cover_Size);
    float d_Cover = sphere(uv_ring);
    float c = GetAlpha(d_Cover- Texture_Size*2./Cover_Size);
    
    float ring_black = GetAlpha(d_Cover- 270./Cover_Size);
    float ring_disk = GetAlpha(d_Cover- 375./Cover_Size);
    float ring_white = GetAlpha(d_Cover- 390./Cover_Size);
    float ring_disk_anisotropic  = smoothstep(0.68,0.9,0.386+abs(atan(uv_ring.y,abs(uv_ring.x))/PI));
    
    vec3 BackGround_tex = GetBackGround(uv);
    vec3 Cover_tex = GetCoverTex(U);

    BackGround_tex = mix(BackGround_tex,vec3(1.,1.,1.),ring_white*0.1);
    BackGround_tex = mix(BackGround_tex,(1.-ring_black)*ring_disk_anisotropic/7.5+vec3(sin(d_Cover*400.+0.1)/70.),ring_disk*0.97);
    BackGround_tex = mix(BackGround_tex,vec3(0.,0.,0.),ring_black*5.);

    Cover_tex = mix(Cover_tex,vec3(0.1)+BackGround_tex,1.-c);
    return Cover_tex;
}

vec2 Tin_Offset = vec2(260.,600.);
vec4 GetTin(vec2 U){
    vec2 uv = U/R;
    U -= Tin_Offset*(R.y/Cover_Size);
	U += Cover_Radius*(R.y/Cover_Size);
    
    vec2 uv_Tin = (U+U)/R.y-(Texture_Size*2./Cover_Size);
	float d_Tin = sphere(uv_Tin);
    float l_Tin = line(uv_Tin,vec2(3.,-10.)/Cover_Size,vec2(72.2,-170.)/Cover_Size);
  	//Bezier!!!!!
    float b_Tin = bezier(uv_Tin,vec2(72.62,-170.)/Cover_Size,vec2(85.6,-199.)/Cover_Size,vec2(123,-226.)/Cover_Size);
    uv_Tin = Rotation(uv_Tin,0.52);
    float box_Tin_1 = box(uv_Tin-vec2(260.,-134.)/Cover_Size,vec2(27.,5.)/Cover_Size);
    float line_Tin_1_1 = line(uv_Tin,vec2(215.,-133.)/Cover_Size,vec2(240.,-134.)/Cover_Size);
    float box_Tin_2 = box(uv_Tin-vec2(315.,-134.)/Cover_Size,vec2(15.,15.)/Cover_Size);
    float line_Tin_2_1 = line(uv_Tin,vec2(306.,-146.)/Cover_Size,vec2(325.,-146.)/Cover_Size);
    float line_Tin_2_2 = line(uv_Tin,vec2(306.,-122.)/Cover_Size,vec2(325.,-122.)/Cover_Size);

    
    float ring_white = GetAlpha(d_Tin- 23./Cover_Size);
    float ring_black = GetAlpha(d_Tin- 8./Cover_Size);
    float ring_white_tiny = GetAlpha(d_Tin- 35./Cover_Size);
    float line_white = GetAlpha(l_Tin- 7./Cover_Size);
    float bezier_white = GetAlpha(b_Tin- 7./Cover_Size);
	float line_white_1_1 = GetAlpha(line_Tin_1_1- 1./Cover_Size);
	float box_white_1 = GetAlpha(box_Tin_1- 7./Cover_Size);
	float box_white_2 = GetAlpha(box_Tin_2- 7./Cover_Size);
    float line_white_2_1 = GetAlpha(line_Tin_2_1- 1./Cover_Size);
    float line_white_2_2 = GetAlpha(line_Tin_2_2- 1./Cover_Size);
    
    vec3 tin_col = vec3(0.);
    tin_col = mix(tin_col,vec3(1.,1.,1.),line_white);
    tin_col = mix(tin_col,vec3(0.65,0.65,0.65),ring_white_tiny);
    tin_col = mix(tin_col,vec3(1.,1.,1.),ring_white);
    tin_col = mix(tin_col,vec3(0.45,0.45,0.45),ring_black);
    tin_col = mix(tin_col,vec3(1.,1.,1.),bezier_white);
    tin_col = mix(tin_col,vec3(0.3,0.3,0.3),line_white_1_1);
    tin_col = mix(tin_col,vec3(1.,1.,1.),box_white_1);
    tin_col = mix(tin_col,vec3(1.,1.,1.),box_white_2);
    tin_col = mix(tin_col,vec3(0.4,0.4,0.4),line_white_2_1);
    tin_col = mix(tin_col,vec3(0.4,0.4,0.4),line_white_2_2);

    float alpha = ring_black + ring_white + line_white + ring_white_tiny*0.2
        + bezier_white + box_white_1 + box_white_1 + box_white_2
        + line_white_2_1 + line_white_2_2;
    return vec4(tin_col,alpha);
}

float GetFrontAlpha(vec2 uv,vec2 index){
    //uv.x *= R.x/R.y;
    if(any(bvec4(greaterThanEqual(uv,vec2(0.0625)),lessThanEqual(uv,vec2(0.)))))
	    return 0.;
    vec2 d = texture(iChannel1,uv+index*0.0625).wx;
	//return smoothstep(0.52,0.47,d.x)*0.5+smoothstep(0.,1.0,d.y)*0.5;
	return d.y;
}

float GetTime(float second,float level){
	float b = second;//秒
    if(level >= 1.){ //十秒
    	second /= 10.;
        b = mod(second,6.);
    }
    if(level >= 2.){ //分
    	second /= 6.;
        b = second;
    }
    if(level >= 3.){ //十分
    	second /= 10.;
        b = mod(second,6.);
    }
    if(level >= 4.){ //时
		second /= 6.;
        b = second;
    }
    return floor(mod(b,10.));
}

vec4 GetPlayerUI(vec2 U){
    vec2 uv = U/R*1.4 - vec2(70.,40.)/Cover_Size;
	
    vec2 offset = vec2(-10.,2.)/Cover_Size;
    float ui_play = GetFrontAlpha(uv-offset,vec2(6.,15.));
    float ui_left = GetFrontAlpha(uv-offset-vec2(-0.07,0.),vec2(1.,15.));
    float ui_right = GetFrontAlpha(uv-offset-vec2( 0.07,0.),vec2(7.,15.));
	//float ui_rest = GetFrontAlpha(uv-offset-vec2(-0.14,0.),vec2(4.,15.));
    float ui_panel = GetAlpha(uv.y-55./Cover_Size);
 	float ui_panel_1 = GetAlpha(uv.y+20./Cover_Size);
    // range at 0-500
    float t = floor(500.* fract(A_time/344.));
	vec2 uv_Slider = uv/vec2(1.,R.x/R.y);
    float ui_line = GetAlpha(line(uv_Slider,vec2(160.,10.)/Cover_Size,vec2(660.,10.)/Cover_Size)-2./Cover_Size);
    float ui_line_1 = GetAlpha(line(uv_Slider,vec2(160.,10.)/Cover_Size,vec2(160. + t,10.)/Cover_Size)-1.7/Cover_Size);
    float d_slip = sphere(uv_Slider-vec2(160.+t,10.)/Cover_Size);
    
    float ui_slip = GetAlpha(d_slip - 4./Cover_Size);
    float ui_slip_1 = GetAlpha(d_slip - 1./Cover_Size);
    float ui_slip_2 = GetAlpha(d_slip - 6./Cover_Size);
    
    vec2 uv_Time = uv_Slider*2.;vec2 offset_left = vec2(-8.,0.)/Cover_Size*2.;
    float ui_time_left_0 = GetFrontAlpha(uv_Time-offset_left-vec2(100.,3.)/Cover_Size*2.,vec2(GetTime(A_time,3.),12.));
    float ui_time_left_1 = GetFrontAlpha(uv_Time-offset_left-vec2(110.,3.)/Cover_Size*2.,vec2(GetTime(A_time,2.),12.));
    float ui_time_left_2 = GetFrontAlpha(uv_Time-offset_left-vec2(120.,4.)/Cover_Size*2.,vec2(10.,12.));
    float ui_time_left_3 = GetFrontAlpha(uv_Time-offset_left-vec2(130.,3.)/Cover_Size*2.,vec2(GetTime(A_time,1.),12.));
    float ui_time_left_4 = GetFrontAlpha(uv_Time-offset_left-vec2(140.,3.)/Cover_Size*2.,vec2(GetTime(A_time,0.),12.));
	vec2 offset_right = vec2(570,0.)/Cover_Size*2.;
    float ui_time_right_0 = GetFrontAlpha(uv_Time-offset_right-vec2(100.,3.)/Cover_Size*2.,vec2(0.,12.));
    float ui_time_right_1 = GetFrontAlpha(uv_Time-offset_right-vec2(110.,3.)/Cover_Size*2.,vec2(5.,12.));
    float ui_time_right_2 = GetFrontAlpha(uv_Time-offset_right-vec2(120.,4.)/Cover_Size*2.,vec2(10.,12.));
    float ui_time_right_3 = GetFrontAlpha(uv_Time-offset_right-vec2(130.,3.)/Cover_Size*2.,vec2(4.,12.));
    float ui_time_right_4 = GetFrontAlpha(uv_Time-offset_right-vec2(140.,3.)/Cover_Size*2.,vec2(4.,12.));
    
    float ui_right_5 = GetFrontAlpha((1.-uv_Slider)*1.2+vec2(98,-343)/Cover_Size*2.,vec2(15.,14.));


    vec3 UI_col = vec3(0.);
    UI_col += vec3(0.2,0.22,0.2)*ui_panel;
    UI_col += vec3(0.2,0.22,0.2)*ui_panel_1 * uv.x*(1.-uv.x)*1.5;
    UI_col += vec3(0.55,0.55,0.55) * ui_play;
    UI_col += vec3(0.55,0.55,0.55) * ui_left;
    UI_col += vec3(0.55,0.55,0.55) * ui_right;
    //UI_col += 0.8 * ui_rest;
    UI_col += vec3(0.8) * ui_line;
    UI_col = mix(UI_col,vec3(1.,0.,0.4) , ui_line_1);
    UI_col = mix(UI_col,vec3(0.2,0.2,0.2), ui_slip_2*0.25);
    UI_col = mix(UI_col,vec3(0.9),ui_slip);
    UI_col = mix(UI_col,vec3(1.,0.,0.7), ui_slip_1);
    
    UI_col += ui_time_left_0;
    UI_col += ui_time_left_1;
    UI_col += ui_time_left_2;
    UI_col += ui_time_left_3;
    UI_col += ui_time_left_4;
    UI_col += ui_time_right_0;
    UI_col += ui_time_right_1;
    UI_col += ui_time_right_2;
    UI_col += ui_time_right_3;
    UI_col += ui_time_right_4;
    UI_col += ui_right_5;
    
    float UI_alpha = (ui_play + ui_left + ui_right)*(uv.y*uv.y)*500.
        + ui_panel*0.8 + ui_panel_1*0.1 + ui_slip 
        + ui_time_left_0 + ui_time_left_1 + ui_time_left_2 + ui_time_left_3 + ui_time_left_4
        + ui_time_right_0 + ui_time_right_1 + ui_time_right_2 + ui_time_right_3 + ui_time_right_4
        + ui_right_5;//+ ui_rest;
    return vec4(UI_col,UI_alpha);
}
//------------------------------------------------
//=================Unicode Font===============================================
/*
	https://www.shadertoy.com/view/XsGXzt by 834144373(恬纳微晰)
	https://github.com/TNWX-Z/Unicode-For-Shadertoy
*/
vec2 FontUV = vec2(0.);
int num4row = int(1.);
#define start_pos(where_x,where_y) FontUV.x=FontBaseUV.x-=where_x;FontUV.y-=16.*(float(num4row)-1.)-where_y;
int adjacency_width = 2;
int adjacency_height= 0;
#define first_word_pos(with) FontUV.x-= float(15+adjacency_width)*float(with);
struct DataFont{
    vec3 a,c;
    vec2 b,d;
};
DataFont F;
#define Font FontCol+=font(F);
#define next FontUV.x = FontBaseUV.x;FontUV.y += 16.+ float(adjacency_height);
#define Next(step) FontUV.x = FontBaseUV.x;FontUV.y += 16.*float(step)+ float(adjacency_height);
#define T F.a=vec3(0x80,0xff,0x427f00);F.b=vec2(0x42c242,0x7f42);F.c=vec3(0x700,0x408ff,0x242400);F.d=vec2(0x447f24,0x444c4);Font
#define N F.a=vec3(0x44e644,0x4848,0x2010ff);F.b=vec2(0x8000c0,0xfe0162);F.c=vec3(0x350c04,0x8c6,0x8080f);F.d=vec2(0x8ff08,0xf0808);Font
#define W F.a=vec3(0xff0080,0x7c0201,0x7c4040);F.b=vec2(0xe61108,0x1e618);F.c=vec3(0x231108,0x7901cc,0x9f909);F.d=vec2(0x1b0479,0x80fe8);Font
#define X F.a=vec3(0x808fc,0x8060fc,0x4180ff);F.b=vec2(0xfe,0xff);F.c=vec3(0x21213f,0x9083f,0x8ff);F.d=vec2(0x22223f,0x24243);Font
#define J_1 F.a=vec3(0xF80C08,0x221010,0x3F2824);F.b=vec2(0x2478A1,0x21312A);F.c=vec3(0xFF0404,0x200404,0x29292F);F.d=vec2(0x2FF92F,0x2F2929);Font
#define J_2 F.a=vec3(0,0x1008FC,0x804020);F.b=vec2(0,0);F.c=vec3(0x100000,0xF,0);F.d=vec2(0x40201,0);Font
#define J_3 F.a=vec3(0x820200,0x880404,0xD02050);F.b=vec2(0x8,0);F.c=vec3(0,0x780601,0x101010);F.d=vec2(0x103C13,0);Font
#define J_4 F.a=vec3(0,0x202FE,0x2222E2);F.b=vec2(0x2E222,0x202);F.c=vec3(0x40404,0x4047F,0x404FF);F.d=vec2(0x4FF04,0x40404);Font
#define J_5 F.a=vec3(0x201010,0x784621,0x800080);F.b=vec2(0x20407F,0x101020);F.c=vec3(0,0x49497F,0x497F49);F.d=vec2(0x7F4949,0);Font
#define J_6 F.a=vec3(0,0x80402,0x806010);F.b=vec2(0,0);F.c=vec3(0,0,0x10000);F.d=vec2(0x3806,0);Font
#define J_7 F.a=vec3(0x487800,0x17A48,0x100FE);F.b=vec2(0xC03806,0x10638);F.c=vec3(0x555D41,0x415D55,0x4417D);F.d=vec2(0x13F018,0x181410);Font
#define S_1 F.a=vec3(0x7E2020,0x80,0x2020F0);F.b=vec2(0x102F0,0xFE);F.c=vec3(0x400608,0x200031,0x242427);F.d=vec2(0x202027,0x20203F);Font
#define	S_2 F.a=vec3(0x222302,0x24FC22,0x2424);F.b=vec2(0xFE0102,0x8000);F.c=vec3(0x497F00,0x497F49,0x7F49);F.d=vec2(0x4B5242,0x36256);Font
#define S_3 F.a=vec3(0,0x800000,0x604080);F.b=vec2(0x98,0);F.c=vec3(0x80000,0x80808,0x80808);F.d=vec2(0x1C0B08,0x8);Font
#define S_4 F.a=vec3(0,0xC10100,0x300C02);F.b=vec2(0xC0,0);F.c=vec3(0,0x3F0000,0);F.d=vec2(0x7F,0);Font
#define S_5 F.a=vec3(0,0xC0200,0xC030);F.b=vec2(0,0);F.c=vec3(0x40400,0x40404,0x47F04);F.d=vec2(0x40404,0x40C);Font

float Bin(float n,float _u,float v,inout float c){
    return (_u>=0. && v>=0. && _u<=2. && v<=7.) ? mod(floor(n/(exp2(8.*_u+v))),2.) : c;
}
float arrayBin(DataFont f){
    float c = 0.,o = 3., y = FontUV.y-8.;
    c = Bin(f.a.r,FontUV.x,FontUV.y,c);c = Bin(f.c.r,FontUV.x,y,c);
    FontUV.x -= o;c = Bin(f.a.g,FontUV.x,FontUV.y,c);c = Bin(f.c.g,FontUV.x,y,c);
    FontUV.x -= o;c = Bin(f.a.b,FontUV.x,FontUV.y,c);c = Bin(f.c.b,FontUV.x,y,c);
    FontUV.x -= o;c = Bin(f.b.r,FontUV.x,FontUV.y,c);c = Bin(f.d.r,FontUV.x,y,c);
    FontUV.x -= o;c = Bin(f.b.g,FontUV.x,FontUV.y,c);c = Bin(f.d.g,FontUV.x,y,c);
    return c;
}
float font(DataFont f){
    FontUV.x -= 3.+ float(adjacency_width);
    return arrayBin(f);
}
vec4 GetRunMarquee(vec2 U){
    float FontCol = 0.;
    float time_wrap = sin(iTime*0.2)*70. + sin(U.y)*clamp((sin(iTime*2.)+0.3)*500.,0.,1.);
    vec2 FontBaseUV = U-0.5 - floor(vec2(530.-time_wrap,340.)*(R/vec2(800.,450.)));
	FontUV = FontBaseUV;
    start_pos(0.,10.);
    Next(0)
        first_word_pos(0)
        J_1 J_2 J_3 J_4 J_5 J_6 J_7
        first_word_pos(3)
        S_1 S_2 S_3 S_4 S_5
    return vec4(0.9,0.9,0.9,FontCol);
}
/*----------------------------------------------------
From https://www.shadertoy.com/view/XdX3z2 by guycook 
----------------------------------------------------*/
#define bars 50.0                 // How many buckets to divide spectrum into
#define barSize 1.0 / bars        // Constant to avoid division in main loop
#define barGap 0.1 * barSize      // 0.1 represents gap on both sides, so a bar is
#define sampleSize 0.02           // How accurately to sample spectrum, must be a factor of 1.0
float h2rgb(float h) {
	if(h < 0.0) h += 1.0;
	if(h < 0.166666) return 0.1 + 4.8 * h;
	if(h < 0.5) return 0.9;
	if(h < 0.666666) return 0.1 + 4.8 * (0.666666 - h);
	return 0.1;
}
vec3 intensityToColour(float i) {
	float h = 0.666666 - (i * 0.666666);
	return vec3(h2rgb(h + 0.333333), h2rgb(h), h2rgb(h - 0.333333));
}
vec4 GetVisual( vec2 U ) {
	vec2 uv = U.xy / R.xy*vec2(2.,2.4)-vec2(550.,400.)/Cover_Size;
    if(any(bvec4(greaterThan(uv,vec2(1.)),lessThan(uv,vec2(0.))))){
    	return vec4(0.);
    }
    float barStart = floor(uv.x * bars) / bars;
	if(uv.x - barStart < barGap || uv.x > barStart + barSize - barGap) 
		return vec4(0.0);
    else
    {
        float intensity = 0.0;
        for(float s = 0.0; s < barSize; s += barSize * sampleSize)
            intensity += texture(iChannel2, vec2(barStart + s, 0.0)).r;
		intensity *= sampleSize; 
		intensity = clamp(intensity, 0.005, 1.0); 
		float i = float(intensity > uv.y);
		return vec4(clamp(intensityToColour(intensity)*0.9,0.,1.) * i, clamp(i * uv.y*5.0,0.,1.));
    }
}

void mainImage( out vec4 C, in vec2 U )
{
    C = GetCover(U).rgbb;
	vec4 Tin = GetTin(U);
    C.rgb = mix(C.rgb,Tin.rgb,Tin.a);
	vec4 UI = GetPlayerUI(U);
    C.rgb = mix(C.rgb,UI.rgb,UI.a);
    vec4 Visual = GetVisual(U);
    C.rgb = mix(C.rgb,Visual.rgb,Visual.a);
    vec4 RunMarquee = GetRunMarquee(U);
    C.rgb = mix(C.rgb,RunMarquee.rgb,RunMarquee.a);
}





/*---------- Shader Story ------------
	Wow!You are so lucky to see the Shader Story!
	I hidden it in a inconspicuous place,but you found.
	
	ATITC(ATC) is also 4bpp texture compression,hum...but it seems that is a black technology.
	And I can't find more about ATC ,so I decided crack it at 2017.12.15,
	It takes me around one week,but at finally I crake it a success.
	In short, it's not smart encode format than DXTn,but it has a higher PSNR.

	This 834144373's Audio Player cover texture is from "NieR:Automata",
	and the audio is also from it;I like the music it's so good at the game.
	The player is inspire from 网易云 music mobile player.
*/