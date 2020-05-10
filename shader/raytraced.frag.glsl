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



uniform vec2 iResolution;
uniform float iTime;
uniform sampler3D hdrCubeMap;
uniform int nbSample;
uniform int size;

in vec2 iTexCoord;

//out vec4 fragColor;

#define FLT_MAX 3.402823466e+38
const float PI  = 3.14159265359;
const float PHI = 1.61803398875;

float loopCount = 0.;

struct ray {
    vec3 A;
    vec3 B;
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

struct sphere{
    vec4 pos;
	vec4 rmfr; //raduis material fuzz refractionIndex
	vec4 color;
};

layout (std430,binding=0) buffer testBufer {
	sphere s[];
};
           
vec3 pointAtParameter(ray r, float t) { return r.A + t*r.B; }

bool hitSphere(in ray r, float tmin, float tmax, inout hitRecord rec, sphere s)
{
    vec3 oc = r.A - s.pos.xyz;
    float a = dot(r.B,r.B);
    float b = dot(oc, r.B);
    float c = dot(oc,oc)-s.rmfr.x*s.rmfr.x;
    float d = b*b - a*c;
    if (d > 0.) 
    {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = pointAtParameter(r,rec.t);
            rec.normal = (rec.p - s.pos.xyz) / s.rmfr.x;
            rec.mat = s.rmfr.y;
            rec.color = s.color.xyz;
			rec.fuzz = s.rmfr.z;
			rec.refaction = s.rmfr.w;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = pointAtParameter(r,rec.t);
            rec.normal = (rec.p - s.pos.xyz) / s.rmfr.x;
            rec.mat = s.rmfr.y;
            rec.color = s.color.xyz;
			rec.fuzz = s.rmfr.z;
			rec.refaction = s.rmfr.w;
            return true;
        }
    }
    return false;
}

bool hitTriangle(in ray r, vec3 v0, vec3 v1, vec3 v2,float tmax, inout hitRecord rec) {

	vec3 e1,e2,h,s,q;
	float a,f,u,v, t;
	e1 = v1 - v0;
	e2 = v1 - v2;

	h =  cross(r.B,e2);
	a = dot(e1,h);

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1/a;
	s = r.A - v0;
	u = f * (dot(s,h));

	if (u < 0.0 || u > 1.0)
		return false;

	q = cross(s,e1);
	v = f * dot(r.B,q);

	if (v < 0.0 || u + v > 1.0)
		return false;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * dot(e2,q);

	if (t > 0.00001 && t < tmax) // ray intersection
	{
		rec.t = t;
        rec.p = pointAtParameter(r,rec.t);
        rec.normal = normalize(cross(v1 - v0, v2 - v0));
		rec.mat = 1;
        rec.color = vec3(0.8, 0.8, 0.0);
		rec.fuzz = 0.0;
		rec.refaction = 1.4;
		return true ;
	}
	else // this means that there is a line intersection
		 // but not a ray intersection
		 return false;

}

bool hit(in ray r, float tmin, float tmax, inout hitRecord rec)
{
    hitRecord tempRec;
    bool hitAny = false;
    float closestSoFar = tmax;
    for(int i = 0; i < size; i++)
    {
        if(hitSphere(r,tmin, closestSoFar,tempRec, s[i]))
        {
            hitAny = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

	vec3 v0 = vec3(-5, 0, 0);
	vec3 v1 = vec3(5, 0, 0);
	vec3 v2 = vec3(0, -5 * sqrt(2), 0);

	if(hitTriangle(r, v0, v1, v2, closestSoFar, tempRec))
	{
		hitAny = true;
		closestSoFar = tempRec.t;
		rec = tempRec;
	}

    return hitAny;
}

float random (vec2 st) {
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(st.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
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
		cosine = refractiveIndex * dot(r.B, rec.normal) / length(r.B);
	}
	else
	{
		outwardNormal = rec.normal;
		niOverNt = 1.0 / refractiveIndex;
		cosine = -dot(r.B, rec.normal) / length(r.B);
	}
	if (checkRefract(unitDirection, outwardNormal, niOverNt))
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
		unitDirection = normalize(r.B);
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
    
    unitDirection = normalize(r.B);
    t =  (unitDirection.y + 1.);
    return att * (t*vec3(0.6,0.8,1.));
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 st = gl_FragCoord.xy/iResolution.y;

    vec3 col = vec3(0.);
    
    for( float x = 0.; x < float(nbSample); x++)
    {
	ray r = getRay(st + x);
        col += color(r,st + x);
		loopCount++;
    }
    
    col = col / float(nbSample);
    col = sqrt(col);

    // Output to screen
    fragColor = col;
}