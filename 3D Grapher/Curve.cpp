#include "Curve.h"
float t;

const Vertex* Curve::curveVert()
{
	Vertex* curve = new Vertex[2 * (m_samples + 1)];

	for (int i = 0; i <= m_samples; i++) {
		t = i * m_sampleLen + m_minPara;
		curve[2 * i + 0] = { vec3(cosf(t), sinf(t), t) - m_width / 2, m_color, 1 };
		curve[2 * i + 1] = { vec3(cosf(t), sinf(t), t) + m_width / 2, m_color, 1 };
	}

	return curve;
}

const unsigned int* Curve::curveIndex()
{
	unsigned int* curve = new unsigned int[6 * m_samples];

	for (int i = 0; i < m_samples; i++) {
		curve[6 * i + 0] = 2 * i + 0;	curve[6 * i + 1] = 2 * i + 2;	curve[6 * i + 2] = 2 * i + 3;
		curve[6 * i + 3] = 2 * i + 3;	curve[6 * i + 4] = 2 * i + 1;	curve[6 * i + 5] = 2 * i + 0;
	}

	return curve;
}

const int Curve::VBOsize()
{
	return 2 * (m_samples + 1);
}

const int Curve::IBOsize()
{
	return 6 * m_samples;
}
