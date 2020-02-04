#pragma once

#include "glm.hpp"
#include "Vertex.h"
#include "VectorFunction.h"

using namespace glm;

class Curve {

public:

	Curve(float minPara = 0.0f, float maxPara = 1.0f, int samples = 16, float width = .05f, vec4 color = vec4(0.0f, 0.0f, 1.0f, 1.0f)) {
		m_minPara = minPara;
		m_maxPara = maxPara;
		m_samples = samples;
		m_sampleLen = ((maxPara - minPara) / samples);
		m_width = width;
		m_color = color;
	}

	const Vertex* curveVert();
	const unsigned int* curveIndex();

	int VBOsize();
	int IBOsize();

private:

	float m_minPara;
	float m_maxPara;
	int m_samples;
	float m_sampleLen;
	float m_width;
	vec4 m_color;
};