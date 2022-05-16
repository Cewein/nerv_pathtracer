//? #version 450
//? #include "data.glsl"

vec3 pointAtParameter(ray r, float t) 
{
	return r.origin + t*r.direction; 
}

bool hitSphere(in ray r, float tmin, float tmax, inout hitRecord hit, sphere s)
{
	vec3 oc = r.origin - s.pos.xyz;
    float a = dot(r.direction,r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc,oc)-s.pos.w*s.pos.w;
    float d = b*b - a*c;
    if (d > 0.) 
    {
        float temp = (-b - sqrt(b*b-a*c))/a; 
        if(temp < tmax && temp > tmin)
        {
            hit.t = temp;
            hit.p = pointAtParameter(r,hit.t);
            hit.normal = (hit.p - s.pos.xyz) / s.pos.w;
            hit.mat = matArr[s.mat];
            return true;
        }
    }
    return false;
}

bool hitRectangle(ray r, float tmin, float tmax, inout hitRecord hit, rectangle rect)
{
	float t = (rect.k-r.origin.z)/r.direction.z;
	if(t < tmin || t > tmax)
		return false;
	float x = r.origin.x + t*r.direction.x;
	float y = r.origin.y + t*r.direction.y;
	if(x < rect.x.x || x > rect.x.y || y < rect.y.x || y > rect.y.y)
		return false;
	hit.t = t;
	hit.normal = vec3(0.0,0.0,1.0);
	hit.p = pointAtParameter(r,hit.t);
	hit.mat = matArr[rect.mat];
	return true;
}

bool hitTriangle(ray r, triangle tris, float tmax, inout hitRecord hit)
{

	vec3 v0 = tris.v1.xyz;
	vec3 v1 = tris.v2.xyz;
	vec3 v2 = tris.v3.xyz;
	//find vectors for two edges sharing vert0
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

		//using barycentric coordinates we can get good weight for each vertex
        float bottom =((v1.y-v2.y)*(v0.x-v2.x)+(v2.x-v1.x)*(v0.y-v2.y));
        float w1 = ((v1.y-v2.y)*(hit.p.x-v2.x)+(v2.x-v1.x)*(hit.p.y-v2.y))
                    /bottom;
        float w2 = ((v2.y-v0.y)*(hit.p.x-v2.x)+(v0.x-v2.x)*(hit.p.y-v2.y))
                    /bottom;
        float w3 = 1.0 - w1 - w2;
        
        //mix all the color from the computed weigth
        vec2 uv = (w1*tris.uv1+w2*tris.uv2+w3*tris.uv3)/(w1+w2+w3);

		material mat;

		mat.transmission = 0.0;
        mat.color = texture2D(checkboard,uv).xyz;
		mat.roughness = 0.0;
		mat.refractionIndex = 1.4;
		mat.metallic = 0.0;
		mat.emission = 0.0;

		hit.mat = mat;
		return true;
	}

    return false;

}

bool hitGround(in ray r, float tmax, inout hitRecord hit) 
{ 
    float t = -(r.origin.y-0.1) / r.direction.y;
	if (t > 0.0001 && t < tmax)
	{

		hit.t = t;
        hit.p = pointAtParameter(r,hit.t);
		hit.normal = vec3(0., 1., 0.);

		material mat;

		//make the ground a checkboard
		if(mod(floor(hit.p.xz), 2.0) == vec2(0.) || mod(floor(hit.p.xz), 2.0) == vec2(1.))
		{
			mat.transmission = 0.0;
			mat.metallic = 0.0;
		}
		else
		{
			mat.transmission = 0.0;
			mat.metallic = .95;
		}
        mat.color = vec3(1.0);
		mat.roughness = 0.0;
		mat.refractionIndex = 1.4;

		hit.mat = mat;
		return true;
	}
	return false;
} 

vec3 hitSky(in ray r)
{
	if(!darkmode)
		return texture(background, vec2((atan(r.direction.z, r.direction.x) / 6.283185307179586476925286766559) + 0.5, acos(r.direction.y) / 3.1415926535897932384626433832795)).xyz;
	else
		return vec3(0.0);
}