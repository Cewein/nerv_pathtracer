#version 450 core

in vec3 iColor;
in vec2 iTexCoord;

out vec4 FragColor;

void main()
{
	FragColor = vec4(iColor,1.0);
}