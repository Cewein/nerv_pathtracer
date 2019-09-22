#version 450 core

uniform float iTime;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0f, abs(sin(iTime)), 1.f, 1.0f);
}