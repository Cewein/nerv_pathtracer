#version 450 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec2 screenResolution;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 iColor;
out vec2 iTexCoord;

void main()
{
	gl_Position =  projection * view * transform * vec4(aPos, 1.);
	iTexCoord = aTexCoord;
}