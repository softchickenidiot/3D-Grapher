#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "gtc/constants.hpp"
#include "glm.hpp"

using namespace std;
using namespace glm;

class VectorFunction {

public:

	VectorFunction(string xFunc = "0", string yFunc = "0", string zFunc = "0") {
		m_xFunc = xFunc;
		m_yFunc = yFunc;
		m_zFunc = zFunc;
	}

	void inputParameter(float t);
	void inputParameter(float s, float t);

	vec3 getPosFunc();
	vec3 getTanFunc();
	vec3 getNorFunc();

private:

	vec3 PositionFunc(float t, float s = 0);
	vec3 TangentFunc(float t);
	vec3 NormalFunc(float t);

	float Function(string func, float t, float s = 0);

	string m_xFunc;
	string m_yFunc;
	string m_zFunc;

	vec3 m_r;
	vec3 m_T;
	vec3 m_N;

	float m_c = 1.0f;
	float m_h = .001f;
};