//? #version 450
//? #include "data.glsl"
//? #include "hit.glsl"


bool slabs(in vec3 p0,in  vec3 p1,in ray r,in float tmini,in float tmaxi) {
	vec3 invRaydir = 1.0/r.direction;
	vec3 t0 = (p0 - r.origin) * invRaydir;
	vec3 t1 = (p1 - r.origin) * invRaydir;
	vec3 tmin = min(t0,t1), tmax = max(t0,t1);
	tmini = max(tmin.x,max(tmin.y,max(tmin.z,tmini)));
	tmaxi = min(tmax.x,min(tmax.y,min(tmax.z,tmaxi)));
	return tmini <= tmaxi;
}

float bvhTraversal(in ray r, float tmin, float tmax, inout hitRecord hit, inout bool hitAny)
{
	//stack traversal without pointer
	int stack[32];	
	int stackAdrr = 0;	
	stack[stackAdrr] = 0;

	float closestSoFar = tmax;

	vec3 invDir = 1.0/r.direction;

	while(stackAdrr >= 0) 
	{
		linearBVHNode node = bvh[stack[stackAdrr]];
		stackAdrr -= 1;

		if(slabs(node.pMin.xyz, node.pMax.xyz, r, tmin, closestSoFar))
		{
			if(node.pMin.w == -1.0 || node.pMax.w == -1.0)
			{
				int i = node.primitiveOffset;
				hitRecord tempHit;
				if(hitTriangle(r, tris[i].v1.xyz, tris[i].v2.xyz, tris[i].v3.xyz, closestSoFar, tempHit))
				{
					hitAny = true;
					closestSoFar = tempHit.t;
					hit = tempHit;
				}
			}

			if(node.pMin.w != -1.0)
			{
				stackAdrr += 1;
				stack[stackAdrr] = int(node.pMin.w);
			}
			if(node.pMax.w != -1.0)
			{
				stackAdrr += 1;
				stack[stackAdrr] = int(node.pMax.w);
			}
		}
	}

	return closestSoFar;
}