//? #version 450

// DATA STRUCT
layout (location = 0) out vec4 fragColor;
layout (location = 15) uniform mat4 viewMatrix;
layout (location = 20) uniform vec3 cameraTransform;

//can be remove if we properly use the viewMatrix
layout (location = 25) uniform vec3 up;
layout (location = 26) uniform vec3 front;
layout (location = 27) uniform vec3 right;

layout (location = 28) uniform float fov;
layout (location = 29) uniform float aperture;
layout (location = 30) uniform float focusDistance;
layout (location = 31) uniform ivec2 iResolution;
layout (location = 33) uniform bool moving;

layout (location = 34) uniform float iTime;

uniform sampler2D background;
uniform int nbSphere;

in vec2 iTexCoord;

struct ray {
    vec3 origin;	//origin
    vec3 direction; //direction
};
    
struct material {
	vec4 color;
	float roughness;
	float metallic;
	float refractionIndex;
	float transmission;
};

material matArr[] = material[6]( material(vec4(0.75,0.25,0.25,1.0),0.0,1.0,1.2,0.0),
	material(vec4(0.25,0.75,0.25,1.0),0.0,0.0,1.2,0.0),
	material(vec4(0.75,0.25,0.75,1.0),0.05,0.2,1.2,0.95),
	material(vec4(0.75,0.25,0.25,1.0),0.0,0.0,1.2,1.0),
	material(vec4(1.0,.5,0.5,100.0),0.0,0.0,1.2,0.0),
	material(vec4(5.0,5.0,20.0,1.0),0.0,0.0,1.2,0.0)
);

struct hitRecord {
    float t;
    vec3 p;
    vec3 normal;
    material mat;
};

struct triangle {
	 vec4 v1;
	 vec4 v2;
	 vec4 v3;
	 vec4 data;
 };


 struct rectangle
 {
	vec2 x;
	vec2 y;
	float k;
	int mat;
 };

 struct sphere 
{
    vec4 pos;
	vec4 mat;
};

sphere sphereArr[] = sphere[4](sphere(vec4(3.0,1.5,0.0,1.5),vec4(0)),
	sphere(vec4(-3.0,1.5,0.0,1.5),vec4(1)),
	sphere(vec4(0.0,1.5,3.0,1.5), vec4(2)),
	sphere(vec4(0.0,1.5,-3.0,1.5),vec4(3))
);

 struct linearBVHNode {
	vec4 pMin;
	vec4 pMax;
	int primitiveOffset;
	int secondChildOffset;
	int nPrimitives;
	int axis;
};

layout (std430,binding=0) buffer triangleBuffer {
	triangle tris[];
};

layout (std430,binding=1) buffer colorBuffer {
	vec4 colorBuf[];
};

layout (std430,binding=2) buffer bvhBuffer {
	linearBVHNode bvh[];
};

layout (std430,binding=3) buffer materialBuffer {
	material matBuf[];
};

layout (std430,binding=4) buffer sphereBuffer {
	sphere sphereBuf[];
};


#define FLT_MAX 3.402823466e+38
const float PI  = 3.14159265359;
const float PHI = 1.61803398875; 

//global variable used everywhere
float rand = 0.0;