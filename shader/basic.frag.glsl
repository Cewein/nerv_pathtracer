#version 450 core

in vec3 iColor;
in vec2 iTexCoord;

out vec4 FragColor;

uniform sampler2D simple;

void main()
{
	FragColor = 1. - texture(simple, iTexCoord) * vec4(iColor,1.);
}