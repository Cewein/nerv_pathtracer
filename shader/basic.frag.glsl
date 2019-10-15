#version 450 core

in vec3 iColor;
in vec2 iTexCoord;

out vec4 FragColor;

uniform sampler2D simple;

void main()
{
	FragColor = texture(simple, iTexCoord);
}