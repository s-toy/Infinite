const int font[] = int[](
 0x69f99, 0x79797, 0xe111e, 0x79997, 0xf171f, 0xf1711, 0xe1d96, 0x99f99, 
 0xf444f, 0x88996, 0x95159, 0x1111f, 0x9f999, 0x9bd99, 0x69996, 0x79971,
 0x69b5a, 0x79759, 0xe1687, 0xf4444, 0x99996, 0x999a4, 0x999f9, 0x99699,
 0x99e8e, 0xf843f, 0x6bd96, 0x46444, 0x6942f, 0x69496, 0x99f88, 0xf1687,
 0x61796, 0xf8421, 0x69696, 0x69e84, 0x66400, 0x0faa9, 0x0000f, 0x00600,
 0x0a500, 0x02720, 0x0f0f0, 0x08421, 0x33303, 0x69404, 0x00032, 0x00002,
 0x55000, 0x00000, 0x00202, 0x42224, 0x24442);

const ivec2 MAP_SIZE = ivec2(4,5);

int drawChar( in int ch, in vec2 pos, in vec2 size, in vec2 uv )
{
    uv = vec2(MAP_SIZE)*(uv-pos)/size;

    ivec2 iuv = ivec2(round(uv));
    
    if( iuv.x<0 || iuv.x>MAP_SIZE.x-1 || iuv.y<0 || iuv.y>MAP_SIZE.y-1 ) return 0;

    int index = MAP_SIZE.x*iuv.y + iuv.x;
    
    return (font[ch]>>index)&1;
}

//------------------------------------------------------------------

vec2  charSize;
float spaceSize;
vec2  charPos;
int   chr;

#define DC(a) chr += drawChar( a, charPos, charSize, uv); charPos.x += spaceSize;

#define _A DC(0)
#define _B DC(1)
#define _C DC(2)
#define _D DC(3)
#define _E DC(4)
#define _F DC(5)
#define _G DC(6)
#define _H DC(7)
#define _I DC(8)
#define _J DC(9)
#define _K DC(10)
#define _L DC(11)
#define _M DC(12)
#define _N DC(13)
#define _O DC(14)
#define _P DC(15)
#define _Q DC(16)
#define _R DC(17)
#define _S DC(18)
#define _T DC(19)
#define _U DC(20)
#define _V DC(21)
#define _W DC(22)
#define _X DC(23)
#define _Y DC(24)
#define _Z DC(25)
#define _0 DC(26)
#define _1 DC(27)
#define _2 DC(28)
#define _3 DC(29)
#define _4 DC(30)
#define _5 DC(31)
#define _6 DC(32)
#define _7 DC(33)
#define _8 DC(34)
#define _9 DC(35)
#define _APST DC(36)
#define _PI   DC(37)
#define _UNDS DC(38)
#define _HYPH DC(39)
#define _TILD DC(40)
#define _PLUS DC(41)
#define _EQUL DC(42)
#define _SLSH DC(43)
#define _EXCL DC(44)
#define _QUES DC(45)
#define _COMM DC(46)
#define _FSTP DC(47)
#define _QUOT DC(48)
#define _BLNK DC(49)
#define _COLN DC(50)
#define _LPAR DC(51)
#define _RPAR DC(52)

void Start( void )
{
    chr = 0;
}

float End( void )
{
    return float(chr);
}

void SetSize( float size )
{
    charSize = size*vec2(MAP_SIZE)/float(MAP_SIZE.x);
    spaceSize = charSize.x*1.25;
}

void SetPos( in vec2 pos )
{
    charPos = pos;
}

//===============================================================

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord / iResolution.y;
    
    Start();
    
    SetSize(0.06);
	SetPos(vec2(0.7, 0.85));
    _S _G _A _M _E
    
    SetSize(0.08);
	SetPos(vec2(0.5, 0.65));
    _I _N _F _I _N _I _T _E

	SetSize(0.03);
	SetPos(vec2(0.64, 0.5));
    _M _A _D _E _BLNK _B _Y _BLNK _I _K _U _T _O

	SetSize(0.02);
	SetPos(vec2(0.6, 0.2));
    _QUOT _P _R _E _S _S _BLNK _S _P _A _C _E _BLNK _T _O _BLNK _S _T _A _R _T _QUOT

    float txt = End();        
    
	fragColor = vec4(txt,txt,txt,1.0);
}