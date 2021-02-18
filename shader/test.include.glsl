//this is a test
//? #version 430

struct ray {
    vec3 origin;	//origin
    vec3 direction; //direction
};
    
struct material {
	vec3 color;
	//todo add emitted color
	float roughness;
	float metallic;
	float refractionIndex;
	float transmission;
};

material matArr[] = material[6]( material(vec3(0.75,0.25,0.25),0.0,1.0,1.2,0.0),
	material(vec3(0.25,0.75,0.25),0.0,0.0,1.2,0.0),
	material(vec3(0.75,0.25,0.75),0.05,0.2,1.2,0.95),
	material(vec3(0.75,0.25,0.25),0.0,0.0,1.2,1.0),
	material(vec3(20.0,5.0,5.0),0.0,0.0,1.2,0.0),
	material(vec3(5.0,5.0,20.0),0.0,0.0,1.2,0.0)
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
    vec3 center;
    float radius;
    int mat;
};

sphere sphereArr[] = sphere[4](sphere(vec3(3.0,1.5,0.0),1.5,0),
	sphere(vec3(-3.0,1.5,0.0),1.5,1),
	sphere(vec3(0.0,1.5,3.0),1.5,2),
	sphere(vec3(0.0,1.5,-3.0),1.5,3)
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