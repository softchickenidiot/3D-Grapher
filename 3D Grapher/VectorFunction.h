#pragma once

#include <iostream>
#include <limits>
#include "glm.hpp"

using namespace std;
using namespace glm;

class VectorFunction {

public:

	VectorFunction(const char* xFunc = "t", const char* yFunc = "t", const char* zFunc = "t") {
		m_xFunc = xFunc;
		m_yFunc = yFunc;
		m_zFunc = zFunc;
	}

	vec3 PositionFunc(float t);
	vec3 TangentFunc(float t);
	vec3 NormalFunc(float t);

private:

	float Function(const char* func, float t);

	const char* m_xFunc;
	const char* m_yFunc;
	const char* m_zFunc;

	vec3 m_r;
	vec3 m_T;
	vec3 m_N;

	float m_t = 0.0f;
	float m_c = 1.0f;
	float m_h = .01f;
};