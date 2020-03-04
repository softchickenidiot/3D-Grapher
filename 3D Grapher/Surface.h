#pragma once

#include "glm.hpp"
#include "Vertex.h"
#include "VectorFunction.h"

using namespace glm;

class Surface {

public:
	Surface(VectorFunction* surfaceFunc, float minS, float maxS, float minT, float maxT, int samples = 16) {
		m_surfaceFunc = surfaceFunc;
		m_minS = minS;
		m_maxS = maxS;
		m_sLen = (maxS - minS) / samples;
		m_minT = minT;
		m_maxT = maxT;
		m_tLen = (maxT - minT) / samples;
		m_samples = samples;
	}

	const Vertex* surfaceVert();
	const unsigned int* surfaceIndex();

	int VBOsize();
	int IBOsize();

private:
	VectorFunction* m_surfaceFunc;
	float m_minS, m_maxS, m_minT, m_maxT, m_sLen, m_tLen, s, t;
	int m_samples;
};