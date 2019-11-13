#version 450 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 15) uniform mat4 view;
layout (location = 10) uniform mat4 projection;

uniform vec2 screenResolution;
uniform mat4 transform;

out vec3 iColor;
out vec2 iTexCoord;

void main()
{
	gl_Position =  projection * view * transform * vec4(aPos, 1.);
	iTexCoord = aTexCoord;
}