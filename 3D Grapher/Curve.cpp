#include "Curve.h"

float t;

const Vertex* Curve::curveVert()
{
	Vertex* curve = new Vertex[2 * (m_samples + 1)];

	t = m_minPara;

	cout << 0 << "% " << 0 << " Lines" << '\n';

	for (int i = 0; i <= m_samples; i++) {
		m_curveFunc->inputParameter(t);
		curve[2 * i + 0] = { m_curveFunc->getPosFunc() - (m_width / 2) * cross(m_curveFunc->getTanFunc(),m_curveFunc->getNorFunc()), m_color, 1 };
		curve[2 * i + 1] = { m_curveFunc->getPosFunc() + (m_width / 2) * cross(m_curveFunc->getTanFunc(),m_curveFunc->getNorFunc()), m_color, 1 };
		
		t += m_sampleLen;

		if (((100 * i) / m_samples) != ((100 * (i - 1)) / m_samples)) {
			cout << (100 * i) / m_samples << "% "<< i << " Lines" << '\n' ;
		}
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

int Curve::VBOsize()
{
	return 2 * (m_samples + 1);
}

int Curve::IBOsize()
{
	return 6 * m_samples;
}
