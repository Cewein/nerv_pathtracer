#version 450 core
layout (location = 0) out vec3 fragColor;
layout (location = 20) uniform mat4 cameraTransform;
layout (location = 25) uniform vec3 up;
layout (location = 26) uniform vec3 front;
layout (location = 27) uniform vec3 right;
layout (location = 28) uniform float fov;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler3D hdrCubeMap;

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
    int mat;
    vec3 color;
	float fuzz;
	float refaction;
};

struct sphere 
{
    vec3 center;
    float radius;
    int mat;
    vec3 color;
	float fuzz;
	float refaction;
};
    
struct hitableList {
    sphere[5] sList;
    int size;
};      
           
vec3 origin(ray r) {return r.A; }
vec3 direction(ray r) {return r.B; }
vec3 pointAtParameter(ray r, float t) { return r.A + t*r.B; }

bool hitSphere(in ray r, float tmin, float tmax, inout hitRecord rec, sphere s)
{
    vec3 oc = origin(r) - s.center;
    float a = dot(direction(r),direction(r));
    float b = dot(oc, direction(r));
    float c = dot(oc,oc)-s.radius*s.radius;
    float d = b*b - a*c;
    if (d > 0.) 
    {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = pointAtParameter(r,rec.t);
            rec.normal = (rec.p - s.center) / s.radius;
            rec.mat = s.mat;
            rec.color = s.color;
			rec.fuzz = s.fuzz;
			rec.refaction = s.refaction;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = pointAtParameter(r,rec.t);
            rec.normal = (rec.p - s.center) / s.radius;
            rec.mat = s.mat;
            rec.color = s.color;
			rec.fuzz = s.fuzz;
			rec.refaction = s.refaction;
            return true;
        }
    }
    return false;
}

bool hit(in ray r, float tmin, float tmax, inout hitRecord rec, hitableList list)
{
    hitRecord tempRec;
    bool hitAny = false;
    float closestSoFar = tmax;
    for(int i = 0; i < list.size; i++)
    {
        if(hitSphere(r,tmin, closestSoFar,tempRec, list.sList[i]))
        {
            hitAny = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAny;
}

ray getRay(vec2 uv) 
{
	float scale = tan(fov * 0.5);
	vec2 d = (2.0 * iTexCoord - 1.0);
	d.x *= (iResolution.x / iResolution.y) * scale;
	d.y *= scale;
	
	vec3 origin = (cameraTransform * vec4(0.,0.,0.,1.)).xyz; 

	vec3 direction = normalize(d.x * right + d.y * up + front);
	return ray(origin,direction); 
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

vec3 color(ray r, hitableList list, vec2 st)
{
	hitRecord rec;
	vec3 unitDirection;
	float t;

	vec3 att = vec3(1.);

	int bounceSize = 5;
	int bounce = 0;

	while (hit(r, 0.001, FLT_MAX, rec, list) && bounce < bounceSize)
	{
		unitDirection = normalize(direction(r));
		if (rec.mat == 0)
		{
			vec3 target = rec.p + rec.normal + randInUnitSphere(st);
			r = ray(rec.p, target - rec.p);
			att *= rec.color;
		}
		if (rec.mat == 1)
		{
			vec3 reflected = reflect(unitDirection, rec.normal);
			r = ray(rec.p, reflected + rec.fuzz * randInUnitSphere(st + loopCount));
			att *= rec.color; // if  att *= rec.color * 50; then the shpere become a light source o_O
		}
		if (rec.mat == 2)
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
				cosine = refractiveIndex * dot(direction(r), rec.normal) / length(direction(r));
			}
			else
			{
				outwardNormal = rec.normal;
				niOverNt = 1.0 / refractiveIndex;
				cosine = -dot(direction(r), rec.normal) / length(direction(r));
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
		bounce++;
	}
    
    unitDirection = normalize(direction(r));
    t =  (unitDirection.y + 1.);
    return att * (t*vec3(0.6,0.8,1.));
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 st = gl_FragCoord.xy/iResolution.y;

	hitableList list;
	list.size = 5;
	list.sList = sphere[5](
		sphere(vec3(0., 0., -1.5), 0.5, 0, vec3(0.8, 0.3, 0.3),0.0,0.0),
		sphere(vec3(-1., .0, -1.5), 0.5, 1, vec3(0.8, 0.8, 0.8),0.5,0.),
		sphere(vec3(1.0, .0, -1.5), 0.5, 2, vec3(0.8, 0.6, 0.2),0.0,2.5),
		sphere(vec3(0., -100.5, -1.), 100., 0, vec3(0.8, 0.8, 0.0),0.,0.),
		sphere(vec3(0., 0.5, -10.), 5., 1, vec3(0.8, 0.6, 0.2),0.,0.)
		);;

    ray r = getRay(st);
    vec3 col = vec3(0.);
    
    float sizeBlending = 10.;
    
    for( float x = 0.; x < sizeBlending; x++)
    {
        col += color(r,list,st + x);
		loopCount++;
    }
    
    col = col / sizeBlending;
    col = sqrt(col);

    // Output to screen
    fragColor = col;
}