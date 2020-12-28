#version 450 core
#define DOF

// DATA STRUCT

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
layout (location = 34) uniform float metal;
layout (location = 35) uniform float transmission;
layout (location = 36) uniform float rougthness;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D text;
uniform int nbSample;
uniform int size;
uniform int nbt;
uniform int debug;
uniform int depth;

in vec2 iTexCoord;

#define FLT_MAX 3.402823466e+38
const float PI  = 3.14159265359;
const float PHI = 1.61803398875; 

//global variable used everywhere
float rand = 0.0;

struct ray {
    vec3 origin;	//origin
    vec3 direction; //direction
};
    
struct material {
	vec3 color;
	float roughness;
	float metallic;
	float refractionIndex;
	float transmission;
};

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
          
//HIT FUNCTION

vec3 pointAtParameter(ray r, float t) 
{
	return r.origin + t*r.direction; 
}

bool hitTriangle(ray r, vec3 v0, vec3 v1, vec3 v2, float tmax, inout hitRecord hit)
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
    float inv_det = 1.0 / det;
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
    if (v < 0.0 || u + v > 1.0)
        return false;
    // calculate t, ray intersects triangle
    float t = dot(edge2, qvec) * inv_det;

	// ray intersection
	if (t > 0.00001 && t < tmax) 
	{
		hit.t = t;
        hit.p = pointAtParameter(r,hit.t);
        hit.normal = normalize(cross(v1 - v0, v2 - v0));

		material mat;

		mat.transmission = transmission;
        mat.color = vec3(0.8, 0.4, 0.0);
		mat.roughness = rougthness;
		mat.refractionIndex = 1.4;
		mat.metallic = metal;

		hit.mat = mat;
		return true;
	}

    return false;

}

bool hitGround(in ray r, float tmax, inout hitRecord hit) 
{ 
    float t = -(r.origin.y) / r.direction.y;
	if (t > 0.0001 && t < tmax)
	{

		hit.t = t;
        hit.p = pointAtParameter(r,hit.t);
		hit.normal = vec3(0., 1., 0.);

		material mat;

		if(mod(floor(hit.p.xz), 2.0) == vec2(0.) || mod(floor(hit.p.xz), 2.0) == vec2(1.))
		{
			mat.transmission = 0.0;
			mat.metallic = 0.0;
		}
		else
		{
			mat.transmission = 0.0;
			mat.metallic = 1.0;
		}
        mat.color = vec3(1.0);
		mat.roughness = 0.0;
		mat.refractionIndex = 1.4;

		hit.mat = mat;
		return true;
	}
	return false;
} 

bool slabs(in vec3 p0,in  vec3 p1,in  vec3 rayOrigin,in  vec3 invRaydir, float closestSoFar) {
	vec3 t0 = (p0 - rayOrigin) * invRaydir;
	vec3 t1 = (p1 - rayOrigin) * invRaydir;
	vec3 tmin = min(t0,t1), tmax = max(t0,t1);
	return max(tmin.x,max(tmin.y,tmin.z)) <= min(tmax.x,min(tmax.y,min(tmax.z,closestSoFar)));
}


bool tempRed = false;

bool hit(in ray r, float tmin, float tmax, inout hitRecord hit)
{
    
    bool hitAny = false;
    float closestSoFar = tmax;

	//stack traversal without pointer
	linearBVHNode bvhNode;

	int stack[32];	
	int stackAdrr = 0;	
	stack[stackAdrr] = 0;

	vec3 invDir = 1.0/normalize(r.direction);

	while(stackAdrr >= 0) 
	{
		linearBVHNode bvhNode = bvh[stack[stackAdrr]];
		stackAdrr -= 1;

		if(slabs(bvhNode.pMin.xyz, bvhNode.pMax.xyz, r.origin, invDir, closestSoFar))
		{
			if(bvhNode.pMin.w == -1.0 && bvhNode.pMax.w == -1.0)
			{
				if(debug == 1) tempRed = true;
				int i = bvhNode.primitiveOffset;
				hitRecord tempHit;
				if(hitTriangle(r, tris[i].v1.xyz, tris[i].v2.xyz, tris[i].v3.xyz, closestSoFar, tempHit))
				{
					hitAny = true;
					closestSoFar = tempHit.t;
					hit = tempHit;
				}
				continue;
			}

			if(bvhNode.pMin.w != -1.0)
			{
				stackAdrr += 1;
				stack[stackAdrr] = int(bvhNode.pMin.w);
			}
			if(bvhNode.pMax.w != -1.0)
			{
				stackAdrr += 1;
				stack[stackAdrr] = int(bvhNode.pMax.w);
			}
		}
	}

	hitRecord tempHit;
	if(hitGround(r, closestSoFar, tempHit))
	{
		hitAny = true;
		closestSoFar = tempHit.t;
		hit = tempHit;
	}

    return hitAny;
}

//PSEUDO RANDOM FUNCTION

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

// RAY MATERIAL INTERACTION

vec3 lambert(in hitRecord rec, in vec2 st, inout ray r)
{
	vec3 target = rec.normal + randInUnitSphere(st);
	r = ray(rec.p, target);
	return rec.mat.color;
}

vec3 metalic(in hitRecord rec, in vec3 unitDirection, in vec2 st, inout ray r)
{
	vec3 reflected = reflect(unitDirection, rec.normal);
	r = ray(rec.p, reflected + rec.mat.roughness * randInUnitSphere(st));
	return rec.mat.color; 
}

float checkRefract(vec3 v, vec3 n, float niOverNt)
{
	vec3 uv = v;
	float dt = dot(uv, n);
	float discriminant = 1.0 - niOverNt * niOverNt * (1. - dt * dt);

	return mix(0.0,1.0, discriminant);
}
              
float schlick(float cosine, float refIdx)
{
	float r0 = (1. - refIdx) / (1. + refIdx);
	r0 = r0 * r0;
	return r0 + (1. - r0) * pow((1. - cosine), 5.);
}

ray dieletric(in hitRecord rec, in vec3 unitDirection, in vec2 st, in ray r)
{
	float refractiveIndex = rec.mat.refractionIndex;
	vec3 outwardNormal = rec.normal;
	vec3 reflected = reflect(unitDirection, rec.normal);
	float niOverNt = 1.0 / refractiveIndex;
	float cosine = -dot(r.direction, rec.normal) / length(r.direction);

	if (dot(unitDirection, rec.normal) > 0.)
	{
		outwardNormal = -rec.normal;
		niOverNt = refractiveIndex;
		cosine = refractiveIndex * dot(r.direction, rec.normal) / length(r.direction);
	}

	float refProb = mix(1.0, schlick(cosine, refractiveIndex), checkRefract(unitDirection, outwardNormal, niOverNt + sin(iTime)));

	//look if the ray if refracted of reflected (based on the view direction)
	r = ray(rec.p, 
			mix(reflected,
			refract(unitDirection, outwardNormal, niOverNt),
			step(refProb, rand)
		)
	);

	return r;
}

//TRACER FUNCTION

vec3 trace(ray r, vec2 st)
{
	hitRecord hitRec;
	vec3 unitDirection;
	float t;

	vec3 att = vec3(1.);

	int bounceSize = 5;
	int bounce = 0;

	//extrat space for ray
	ray tmp;
	ray glass;

	while (hit(r, 0.0001, FLT_MAX, hitRec) && bounce < bounceSize)
	{
		unitDirection = normalize(r.direction);

		glass = dieletric(hitRec, unitDirection, st, r);
		tmp = r;

		//get color attenuation by mixing both lambert and metalic mat
		vec3 attenuate = mix(
			lambert(hitRec,st,r), //ray store in r
			metalic(hitRec, unitDirection, st, tmp), //ray store in tmp
			hitRec.mat.metallic
		);

		//choose between attenuation or no attenuation
		att *= mix(
			attenuate,
			vec3(1.0),
			hitRec.mat.transmission
		);

		//get the new direction if the ray it a metalic or lambert mat
		vec3 rayDir = mix(r.direction,tmp.direction,hitRec.mat.metallic);

		//chosse between refraction or reflection
		r.direction = mix(
			rayDir,
			glass.direction,
			hitRec.mat.transmission
		);

		bounce++;
	}
    
    unitDirection = normalize(r.direction);
    t =  (unitDirection.y + 1.);

	//skybox
	//todo reference the shader found on shader toy
	vec3 col = texture(text, -vec2((atan(unitDirection.z, unitDirection.x) / 6.283185307179586476925286766559) + 0.5, acos(unitDirection.y) / 3.1415926535897932384626433832795)).xyz;
    return att * col;
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 st = gl_FragCoord.xy/iResolution.y;

	vec4 cbd = colorBuf[screenWidth * int(gl_FragCoord.y) + int(gl_FragCoord.x)];

    vec3 col = vec3(0.);
    rand = random( st + cbd.w);

	ray r = getRay(st + sin(cbd.w));
    col = trace(r,st + cbd.w);
	col = pow(col, vec3(0.4545));

	if(tempRed) col = vec3(1.0,0.0,0.0);

	if(moving)
		cbd = vec4(col,1.);
	
	//accumulation
	cbd.xyz = cbd.xyz * cbd.w;
	cbd.xyz += col;
	cbd.w += 1;
	cbd.xyz = cbd.xyz / cbd.w;

    // Output to screen and buffer
	colorBuf[screenWidth * int(gl_FragCoord.y) + int(gl_FragCoord.x)] = cbd;
    fragColor = cbd.xyz;
}