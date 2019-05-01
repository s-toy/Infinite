#version 430 core

#define MAX_CHANNEL_NUM 4

uniform float iTime;
uniform float iTimeDelta;

uniform int iAudioDuration;
uniform int iFrame;

uniform vec3 iChannelResolution[MAX_CHANNEL_NUM];
uniform vec3 iResolution;
uniform vec4 iDate;
uniform vec4 iMouse;

uniform sampler2D iChannel[MAX_CHANNEL_NUM];

#define iChannel0 iChannel[0]
#define iChannel1 iChannel[1]
#define iChannel2 iChannel[2]
#define iChannel3 iChannel[3]

in vec2 _TexCoords;

out vec4 _outFragColor;

void mainImage(out vec4 fragColor, in vec2 fragCoord);

void main()
{
	vec2 fragCoord = _TexCoords * iResolution.xy;
	mainImage(_outFragColor, fragCoord);
}