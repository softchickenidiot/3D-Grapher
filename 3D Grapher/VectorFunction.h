#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "glm.hpp"

using namespace std;
using namespace glm;

class VectorFunction {

public:

	VectorFunction(string xFunc = "t", string yFunc = "t", string zFunc = "t") {
		m_xFunc = xFunc;
		m_yFunc = yFunc;
		m_zFunc = zFunc;
	}

	vec3 PositionFunc(float t);
	vec3 TangentFunc(float t);
	vec3 NormalFunc(float t);

private:

	float Function(string func, float t);

	string m_xFunc;
	string m_yFunc;
	string m_zFunc;

	vec3 m_r;
	vec3 m_T;
	vec3 m_N;

	float m_c = 1.0f;
	float m_h = .01f;
};