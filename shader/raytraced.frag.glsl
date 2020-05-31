#version 450 core
#define DOF
layout (location = 0) out vec3 fragColor;
layout (location = 20) uniform mat4 cameraTransform;
layout (location = 25) uniform vec3 up;
layout (location = 26) uniform vec3 front;
layout (location = 27) uniform vec3 right;
layout (location = 28) uniform float fov;
layout (location = 29) uniform float aperture;
layout (location = 30) uniform float focusDistance;
layout (location = 31) uniform int screenWidth;
layout (location = 32) uniform int screenHeight;
layout (location = 33) uniform bool moving;



uniform vec2 iResolution;
uniform float iTime;
uniform sampler3D hdrCubeMap;
uniform int nbSample;
uniform int size;
uniform int nbt;

in vec2 iTexCoord;

//out vec4 fragColor;

#define FLT_MAX 3.402823466e+38
const float PI  = 3.14159265359;
const float PHI = 1.61803398875;

float loopCount = 0.;

struct ray {
    vec3 origin;	//origin
    vec3 direction; //direction
};
    
struct hitRecord {
    float t;
    vec3 p;
    vec3 normal;
    float mat;
    vec3 color;
	float fuzz;
	float refaction;
};

struct triangle {
	 vec4 v1;
	 vec4 v2;
	 vec4 v3;
 };


layout (std430,binding=0) buffer triangleBuffer {
	triangle tris[];
};

layout (std430,binding=1) buffer colorBuffer {
	vec4 colorBuf[];
};
           
vec3 pointAtParameter(ray r, float t) { return r.origin + t*r.direction; }

bool hitTriangle(ray r, vec3 v0, vec3 v1, vec3 v2, float tmax, inout hitRecord rec)
{
	 // find vectors for two edges sharing vert0
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    // begin calculating determinant - also used to calculate U parameter
    vec3 pvec = cross(r.direction, edge2);
    // if determinant is near zero, ray lies in plane of triangle
    float det = dot(edge1, pvec);
    // use backface culling
    if (det < 0.00001)
        return false;
    float inv_det = 1.0f / det;
    // calculate distance from vert0 to ray origin
    vec3 tvec = r.origin - v0;
    // calculate U parameter and test bounds
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0f)
        return false;
    // prepare to test V parameter
    vec3 qvec = cross(tvec, edge1);
    // calculate V parameter and test bounds
    float v = dot(r.direction, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0f)
        return false;
    // calculate t, ray intersects triangle
    float t = dot(edge2, qvec) * inv_det;

	if (t > 0.00001 && t < tmax) // ray intersection
	{
		rec.t = t;
        rec.p = pointAtParameter(r,rec.t);
        rec.normal = normalize(cross(v1 - v0, v2 - v0));
		rec.mat = 2;
		if(rec.p.z < 1.) rec.mat = 0;
		if(rec.p.z < -1.) rec.mat = 1;
        rec.color = vec3(0.8, 0.8, 0.0);
		if(rec.p.z < -1.) rec.color = vec3(0.6, 0.6, 0.3);
		rec.fuzz = 0.;
		rec.refaction = 1.4;
		return true;
	}

    return false;

}

bool hitGround(in ray r, float tmax, inout hitRecord rec) 
{ 
    float t = -(r.origin.y) / r.direction.y;
	if (t > 0.0001 && t < tmax)
	{

		rec.t = t;
        rec.p = pointAtParameter(r,rec.t);
		rec.normal = vec3(0.f, 1.f, 0.f);
		if(mod(floor(rec.p.xz), 2.0) == vec2(0.) || mod(floor(rec.p.xz), 2.0) == vec2(1.))
			rec.mat = 1;
		else
			rec.mat = 0;
        rec.color = vec3(.2);
		rec.fuzz = 0.1;
		rec.refaction = 1.4;
		return true;
	}
	return false;
} 

bool hit(in ray r, float tmin, float tmax, inout hitRecord rec)
{
    hitRecord tempRec;
    bool hitAny = false;
    float closestSoFar = tmax;
    for(int i = 0; i < size; i++)
    {
        if(hitTriangle(r, tris[i].v1.xyz, tris[i].v2.xyz, tris[i].v3.xyz, closestSoFar, tempRec))
		{
			hitAny = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
    }

	if(hitGround(r, closestSoFar, tempRec))
	{
		hitAny = true;
		closestSoFar = tempRec.t;
		rec = tempRec;
	}

    return hitAny;
}

float random (vec2 st) {
	return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 randInUnitSphere(vec2 st) {
    float phi = random(st.yx) * 2.0 * 3.14159265;
    float theta = random(st.xy) * 3.14169265;
    
    return vec3(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta));
}

vec3 randInUnitDisk(vec2 st)
{
	return vec3(random(st.yx),random(st.xy),0.) * 2.0 - 1.0; 
}

#ifdef DOF
ray getRay(vec2 uv) 
{

	
	float scale = tan(fov/180.0);
	vec2 d = (2.0 * iTexCoord - 1.0);
	d.x *= (iResolution.x / iResolution.y) * scale;
	d.y *= scale;

	vec3 origin = (cameraTransform * vec4(0.,0.,0.,1.)).xyz;
	float lensRaduis = aperture/2.0;
	vec3 rd = lensRaduis * randInUnitDisk(uv);
	vec3 offset = up * rd.x + right * rd.y;

	vec3 ft = origin + d.y * focusDistance * up + d.x * focusDistance * right + focusDistance * front;

	vec3 direction = normalize(d.x * right + d.y * up + ft - origin - offset);
	return ray(origin + offset,direction); 
}
#else
ray getRay(vec2 uv) 
{
	float scale = tan(fov/180.0);
	vec2 d = (2.0 * iTexCoord - 1.0);
	d.x *= (iResolution.x / iResolution.y) * scale;
	d.y *= scale;

	vec3 origin = (cameraTransform * vec4(0.,0.,0.,1.)).xyz;

	vec3 direction = normalize(d.x * right + d.y * up + front);
	return ray(origin,direction); 
}
#endif

bool checkRefract(vec3 v, vec3 n, float niOverNt)
{
	vec3 uv = v;
	float dt = dot(uv, n);
	float discriminant = 1.0 - niOverNt * niOverNt * (1. - dt * dt);
	if (discriminant > 0.)
	{
		return true;
	}
	return false;
}
              
float schlick(float cosine, float refIdx)
{
	float r0 = (1. - refIdx) / (1. + refIdx);
	r0 = r0 * r0;
	return r0 + (1. - r0) * pow((1. - cosine), 5.);
}

vec3 lambert(in hitRecord rec, in vec2 st, inout ray r)
{
	vec3 target = rec.p + rec.normal + randInUnitSphere(st);
	r = ray(rec.p, target - rec.p);
	return rec.color;
}

vec3 metalic(in hitRecord rec, in vec3 unitDirection, in vec2 st, inout ray r)
{
	vec3 reflected = reflect(unitDirection, rec.normal);
	r = ray(rec.p, reflected + rec.fuzz * randInUnitSphere(st + loopCount));
	return rec.color; // if  att *= rec.color * 50; then the shpere become a light source o_O
}

void dialetric(in hitRecord rec, in vec3 unitDirection, in vec2 st, inout ray r)
{
	float refractiveIndex = rec.refaction;
	vec3 outwardNormal;
	vec3 reflected = reflect(unitDirection, rec.normal);
	float niOverNt;
	vec3 refracted;

	float refProb;
	float cosine;

	if (dot(unitDirection, rec.normal) > 0.)
	{
		outwardNormal = -rec.normal;
		niOverNt = refractiveIndex;
		cosine = refractiveIndex * dot(r.direction, rec.normal) / length(r.direction);
	}
	else
	{
		outwardNormal = rec.normal;
		niOverNt = 1.0 / refractiveIndex;
		cosine = -dot(r.direction, rec.normal) / length(r.direction);
	}
	if (checkRefract(unitDirection, outwardNormal, niOverNt + sin(iTime)))
	{

		refProb = schlick(cosine, refractiveIndex);
	}
	else
	{
		refProb = 1.0;
	}

	if (random(vec2(loopCount)) < refProb)
	{
				
		r = ray(rec.p, reflected);
	}
	else
	{
		r = ray(rec.p, refract(unitDirection, outwardNormal, niOverNt));
	}
}

vec3 color(ray r, vec2 st)
{
	hitRecord rec;
	vec3 unitDirection;
	float t;

	vec3 att = vec3(1.);

	int bounceSize = 5;
	int bounce = 0;

	while (hit(r, 0.001, FLT_MAX, rec) && bounce < bounceSize)
	{
		unitDirection = normalize(r.direction);
		if (rec.mat == 0.0)
		{
			att *= lambert(rec,st,r);
		}
		if (rec.mat == 1.0)
		{
			att *= metalic(rec, unitDirection, st, r);
		}
		if (rec.mat == 2.0)
		{
			dialetric(rec, unitDirection, st, r);
		}
		bounce++;
	}
    
    unitDirection = normalize(r.direction);
    t =  (unitDirection.y + 1.);
    return att * (t*vec3(0.6,0.8,1.));
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 st = gl_FragCoord.xy/iResolution.y;

	vec4 cbd = colorBuf[screenWidth * int(gl_FragCoord.y) + int(gl_FragCoord.x)];

    vec3 col = vec3(0.);
    loopCount = cbd.w;

	ray r = getRay(st + cos(sin(cbd.w)));
    col = color(r,st + cbd.w);

	col = pow(col, vec3(0.4545));


	if(moving)
		cbd = vec4(col,1.);
	else
	{
		cbd.xyz = cbd.xyz * cbd.w;
		cbd.xyz += col;
		cbd.w += 1;
		cbd.xyz = cbd.xyz / cbd.w;
	}

    // Output to screen and buffer
	colorBuf[screenWidth * int(gl_FragCoord.y) + int(gl_FragCoord.x)] = cbd;
    fragColor = cbd.xyz;
}