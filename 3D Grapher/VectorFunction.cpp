#include "VectorFunction.h"

vec3 r;
vec3 T;
vec3 N;
float h = .005;

vec3 PositionFunction(float t)
{
	r = { cos(t), sin(t), t };

	return r;
}

vec3 TangentFunction(float t)
{
	T = (PositionFunction(t + h) - PositionFunction(t)) / h;
	T += (PositionFunction(t - h) - PositionFunction(t)) / h;
	T /= 2.0f;
	T = normalize(T);
	T = vec3(abs(T.x), abs(T.y), abs(T.z));

	return T;
}

vec3 NormalFunction(float t)
{
	N = (TangentFunction(t + h) - TangentFunction(t)) / h;
	N += (TangentFunction(t - h) - TangentFunction(t)) / h;
	N /= 2.0f;
	N = normalize(N);
	N = vec3(abs(N.x), abs(N.y), abs(N.z));

	return N;
}
