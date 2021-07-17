//? #version 450
//? #include "data.glsl"

//PSEUDO RANDOM FUNCTION

float random (vec2 p) {
	return fract(43757.5453*sin(dot(p, vec2(12.9898,78.233))));
}

vec3 randInUnitSphere(vec2 st) {
    float phi = random(st.yx + sin(iTime) + cos(-iTime/tan(iTime))) * 2.0 * 3.14159265;
    float theta = random(st.xy + cos(iTime/tan(-iTime)) + sin(-iTime)) * 3.14169265;
    return vec3(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta));
}

vec3 randInUnitDisk(vec2 st)
{
	return vec3(random(st.yx + sin(iTime) + cos(-iTime)) ,random(st.xy + cos(iTime) + sin(-iTime)),0.) * 2.0 - 1.0; 
}