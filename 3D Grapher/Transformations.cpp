#include "Transformations.h"

using namespace glm;

const mat4 initScaleTrans(vec3 scale)
{
	mat4 m;

	m[0][0] = scale.x; m[0][1] = 0.0f;		m[0][2] = 0.0f;		m[0][3] = 0.0f;
	m[1][0] = 0.0f;		m[1][1] = scale.y; m[1][2] = 0.0f;		m[1][3] = 0.0f;
	m[2][0] = 0.0f;		m[2][1] = 0.0f;		m[2][2] = scale.z; m[2][3] = 0.0f;
	m[3][0] = 0.0f;		m[3][1] = 0.0f;		m[3][2] = 0.0f;		m[3][3] = 1.0f;

	return m;
}

const mat4 initRotateTrans(vec3 rotateInfo)
{
	mat4 rx, ry, rz, m;
	rotateInfo.x = rotateInfo.x * pi<float>() / 180;
	rotateInfo.y = rotateInfo.y * pi<float>() / 180;
	rotateInfo.z = rotateInfo.z * pi<float>() / 180;

	rx[0][0] = 1.0f;	rx[0][1] = 0.0f;				rx[0][2] = 0.0f;				 rx[0][3] = 0.0f;
	rx[1][0] = 0.0f;	rx[1][1] = cosf(rotateInfo.x);	rx[1][2] = sinf(rotateInfo.x); rx[1][3] = 0.0f;
	rx[2][0] = 0.0f;	rx[2][1] = -sinf(rotateInfo.x); rx[2][2] = cosf(rotateInfo.x);	 rx[2][3] = 0.0f;
	rx[3][0] = 0.0f;	rx[3][1] = 0.0f;				rx[3][2] = 0.0f;				 rx[3][3] = 1.0f;

	ry[0][0] = cosf(rotateInfo.y);	ry[0][1] = 0.0f;	ry[0][2] = sinf(rotateInfo.y); ry[0][3] = 0.0f;
	ry[1][0] = 0.0f;				ry[1][1] = 1.0f;	ry[1][2] = 0.0f;				 ry[1][3] = 0.0f;
	ry[2][0] = -sinf(rotateInfo.y);	ry[2][1] = 0.0f;	ry[2][2] = cosf(rotateInfo.y);	 ry[2][3] = 0.0f;
	ry[3][0] = 0.0f;				ry[3][1] = 0.0f;	ry[3][2] = 0.0f;				 ry[3][3] = 1.0f;

	rz[0][0] = cosf(rotateInfo.z);	rz[0][1] = sinf(rotateInfo.z);	rz[0][2] = 0.0f; rz[0][3] = 0.0f;
	rz[1][0] = -sinf(rotateInfo.z);	rz[1][1] = cosf(rotateInfo.z);		rz[1][2] = 0.0f; rz[1][3] = 0.0f;
	rz[2][0] = 0.0f;				rz[2][1] = 0.0f;					rz[2][2] = 1.0f; rz[2][3] = 0.0f;
	rz[3][0] = 0.0f;				rz[3][1] = 0.0f;					rz[3][2] = 0.0f; rz[3][3] = 1.0f;

	m = rx * ry * rz;
	return m;
}

const mat4 initTranslationTrans(vec3 worldPos)
{
	mat4 m;

	m[0][0] = 1.0f; m[0][1] = 0.0f;	m[0][2] = 0.0f;	m[0][3] = 0.0f;
	m[1][0] = 0.0f;	m[1][1] = 1.0f; m[1][2] = 0.0f;	m[1][3] = 0.0f;
	m[2][0] = 0.0f;	m[2][1] = 0.0f;	m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = worldPos.x;	m[3][1] = worldPos.y;	m[3][2] = worldPos.z;	m[3][3] = 1.0f;

	return m;
}

const mat4 initPersProjection(PersProjInfo persProjInfo)
{
	mat4 m;

	const float ar = persProjInfo.Width / persProjInfo.Height;
	const float zNear = persProjInfo.zNear;
	const float zFar = persProjInfo.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf((persProjInfo.FOV / 2.0f) * pi<float>() / 180);

	m[0][0] = 1.0f / (tanHalfFOV * ar);	m[0][1] = 0.0f;				 m[0][2] = 0.0f;						 m[0][3] = 0.0f;
	m[1][0] = 0.0f;						m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;						 m[1][3] = 0.0f;
	m[2][0] = 0.0f;						m[2][1] = 0.0f;				 m[2][2] = (-zNear - zFar) / zRange;	 m[2][3] = 1.0f;
	m[3][0] = 0.0f;						m[3][1] = 0.0f;				 m[3][2] = (2.0f * zFar * zNear) / zRange; m[3][3] = 0.0f;

	return m;
}

const mat4 initCameraTransform(const vec3 Target, const vec3 Up)
{
	mat4 m;
	vec3 N = Target;
	N = normalize(N);
	vec3 U = Up;
	U = cross(U, N);
	U = normalize(U);
	vec3 V = cross(N, U);

	m[0][0] = U.x; m[0][1] = V.x; m[0][2] = N.x; m[0][3] = 0.0f;
	m[1][0] = U.y; m[1][1] = V.y; m[1][2] = N.y; m[1][3] = 0.0f;
	m[2][0] = U.z; m[2][1] = V.z; m[2][2] = N.z; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return m;
}