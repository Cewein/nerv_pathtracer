#version 450 core

out vec4 fragColor;

in vec2 iTexCoord;

uniform sampler2D text;
uniform sampler2D norm;
uniform sampler2D spec;

void main()
{
	fragColor = vec4(vec3(1. - texture(text, iTexCoord).xyz),1.0);
}
