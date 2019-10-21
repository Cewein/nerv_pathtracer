#version 450 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec2 screenResolution;

out vec3 iColor;
out vec2 iTexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.); 
	iColor = aColor;
	iTexCoord = aTexCoord;
}