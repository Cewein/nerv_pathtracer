#version 450 core

in vec3 iColor;
in vec2 iTexCoord;

out vec4 FragColor;

uniform sampler2D text;
uniform sampler2D norm;
uniform sampler2D spec;

void main()
{
	FragColor = mix(texture(text, iTexCoord),texture(norm, iTexCoord),0.4);
}