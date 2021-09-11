#version 450 core
#define DOF       

#include "data.glsl"
#include "commun.glsl"
#include "hit.glsl"
#include "bvh.glsl"


bool noScatter = false;

bool hit(in ray r, float tmin, float tmax, inout hitRecord hit)
{
    
    bool hitAny = false;
    float closestSoFar = tmax;

	closestSoFar = bvhTraversal(r, tmin, closestSoFar, hit, hitAny);

	hitRecord tempHit;
	if(hitGround(r, closestSoFar, tempHit))
	{
		hitAny = true;
		closestSoFar = tempHit.t;
		hit = tempHit;
	}



	for(int i = 0; i < 10; i++)
	{
		rectangle rect = rectangle(vec2(-0.25, 0.25) + vec2(0.25,0.25) * i, vec2(0.0, 1.0), -1.5  - 0.15* i, 4);

		if(hitRectangle(r, tmin, closestSoFar, tempHit, rect))
		{
			hitAny = true;
			closestSoFar = tempHit.t;
			hit = tempHit;
			noScatter = true;
		}

		rect = rectangle(vec2(-0.25, 0.25) + vec2(0.25,0.25) * i, vec2(0.0, 1.0), 1.7  + 0.15 * i, 5);

		if(hitRectangle(r, tmin, closestSoFar, tempHit, rect))
		{
			hitAny = true;
			closestSoFar = tempHit.t;
			hit = tempHit;
			noScatter = true;
		}
	}

	sphere s = sphere(vec4(-7.0,0.0,0.0,4.0),vec4(3.0));

	if(hitSphere(r,tmin,closestSoFar,tempHit,s))
	{
		hitAny = true;
		closestSoFar = tempHit.t;
		hit = tempHit;
		noScatter = false;
	}

    return hitAny;
}

//todo a better getRay function since the focus blur/bokey seem wrongly done
ray getRay(vec2 uv) 
{

	vec3 up = viewMatrix[1].xyz;
	vec3 right = viewMatrix[0].xyz;
	vec3 front = viewMatrix[2].xyz;

	float scale = tan(fov/180.0);
	vec2 d = (2.0 * gl_FragCoord.xy/iResolution.x - 1.0);
	d.x *= scale;
	d.y *= scale;

	vec3 origin = (cameraTransform).xyz;
	float lensRaduis = aperture/2.0;
	vec3 rd = lensRaduis * randInUnitDisk(uv);
	vec3 offset = up * rd.x + right * rd.y;

	vec3 ft = origin + d.y * focusDistance * up + d.x * focusDistance * right + focusDistance * front;

	vec3 direction = normalize(d.x * right + d.y * up + ft - origin - offset);
	return ray(origin + offset,direction); 
}

// RAY MATERIAL INTERACTION
vec3 lambert(in hitRecord rec, in vec2 st, inout ray r)
{
	vec3 target = rec.normal + randInUnitSphere(st + r.direction.xy);
	r = ray(rec.p, target);
	return rec.mat.color.xyz * rec.mat.color.w;
}

vec3 metalic(in hitRecord rec, in vec3 unitDirection, in vec2 st, inout ray r)
{
	vec3 reflected = reflect(unitDirection, rec.normal);
	r = ray(rec.p, reflected + rec.mat.roughness * randInUnitSphere(st + r.direction.xy));
	return rec.mat.color.xyz * rec.mat.color.w; 
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
	
	//we should look into the glsl step function to remove this if
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
			step(refProb, random(st.xy + cos(iTime * st.yx)))
		)
	);

	return r;
}

//TRACER FUNCTION
//todo redo this function
//it work but is poorly done and can be way much better
vec3 trace(ray r, vec2 st)
{
	hitRecord hitRec;
	vec3 unitDirection;

	vec3 att = vec3(1.);

	int bounce = 0;

	//extrat space for ray
	ray tmp;
	ray glass;

	while (bounce < maxBounce && !noScatter)
	{
		unitDirection = normalize(r.direction);

		if(hit(r, 0.00001, FLT_MAX, hitRec))
			bounce++;
		else
		{
			vec3 col = vec3(0.0);
			if(!darkmode)
				col = texture(background, vec2((atan(r.direction.z, r.direction.x) / 6.283185307179586476925286766559) + 0.5, acos(r.direction.y) / 3.1415926535897932384626433832795)).xyz;

			att *= col;
			break;
		}

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

		
	}

	//skybox
    return att;
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 st = gl_FragCoord.xy/iResolution;

	vec4 cbd = colorBuf[iResolution.x * int(gl_FragCoord.y) + int(gl_FragCoord.x)];

    vec3 col = vec3(0.);

	ray r = getRay(st + sin(cbd.w));
    col = trace(r,st + cbd.w);
	col = pow(col, vec3(0.4545));

	if(moving)
		cbd = vec4(col,1.);
	
	//accumulation
	cbd.xyz = cbd.xyz * cbd.w;
	cbd.xyz += col;
	cbd.w += 1;
	cbd.xyz = cbd.xyz / cbd.w;

    // Output to screen and buffer
	colorBuf[iResolution.x * int(gl_FragCoord.y) + int(gl_FragCoord.x)] = cbd;
    fragColor = vec4(cbd.xyz,0);
}