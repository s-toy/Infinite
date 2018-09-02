#version 430 core

uniform mat4 uModelMatrix = mat4(1.0);
uniform vec2 iResolution = vec2(1.0);

layout (location = 0) in vec3 _inFragPosition;
layout (location = 1) in vec2 _inTexCoords;

out vec2 fragCoord;

void main()
{
	gl_Position = uModelMatrix * vec4(_inFragPosition, 1.0f);
	fragCoord = _inTexCoords * iResolution;
}