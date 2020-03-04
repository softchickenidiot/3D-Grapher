#include "Surface.h"

const Vertex* Surface::surfaceVert()
{	
	Vertex* surface = new Vertex[pow(m_samples + 1, 2)];

	s = m_minS;
	t = m_minT;

	cout << 0 << "% " << 0 << " Vertices" << '\n';

	for (int i = 0; i <= m_samples; i++) {
		t = m_minT;
		for (int j = 0; j <= m_samples; j++) {

			m_surfaceFunc->inputParameter(s, t);

			surface[(m_samples + 1) * i + j] = { m_surfaceFunc->getPosFunc(), vec4(float(j)/m_samples, float(i)/m_samples, 0.0f, 1.0f), 1};
			/*
			surface[5 * ((m_samples + 1) * i + j) + 1] = { m_surfaceFunc->getPosFunc() - .005 * m_surfaceFunc->getPartialS, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((m_samples + 1) * i + j) + 2] = { m_surfaceFunc->getPosFunc() + .005 * m_surfaceFunc->getPartialS, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((m_samples + 1) * i + j) + 3] = { m_surfaceFunc->getPosFunc() - .005 * m_surfaceFunc->getPartialT, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			surface[5 * ((m_samples + 1) * i + j) + 4] = { m_surfaceFunc->getPosFunc() + .005 * m_surfaceFunc->getPartialT, vec4(0.0f, 0.0f, 0.0f, 1.0f), 1 };
			*/
			cout << '<' << surface[(m_samples + 1) * i + j].m_pos.x << ',' << surface[(m_samples + 1) * i + j].m_pos.y << ',' << surface[(m_samples + 1) * i + j].m_pos.z << ">\n";
			t += m_tLen;
		}

		if (((100 * (m_samples + 1) * i) / m_samples) != ((100 * ((m_samples + 1) * i - 1)) / m_samples)) {
			cout << (100 * (m_samples + 1) * i) / m_samples << "% " << (m_samples + 1) * i << " Vertices" << '\n';
		}
		s += m_sLen;
	}

	return surface;	
}

const unsigned int* Surface::surfaceIndex()
{
	unsigned int* surface = new unsigned int[6 * pow(m_samples, 2)];

	for (int i = 0; i < m_samples; i++) {
		for (int j = 0; j < m_samples; j++) {
			surface[6 * (m_samples * i + j) + 0] = (m_samples + 1) * (i + 0) + j + 0;	surface[6 * (m_samples * i + j) + 1] = (m_samples + 1) * (i + 0) + j + 1;	surface[6 * (m_samples * i + j) + 2] = (m_samples + 1) * (i + 1) + j + 1;
			surface[6 * (m_samples * i + j) + 3] = (m_samples + 1) * (i + 1) + j + 1;	surface[6 * (m_samples * i + j) + 4] = (m_samples + 1) * (i + 1) + j + 0;	surface[6 * (m_samples * i + j) + 5] = (m_samples + 1) * (i + 0) + j + 0;

		}
	}

	/*
	for (int i = 0; i < samples + 1; i++) {
		for (int j = 0; j < samples; j++) {
			sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 0] = 5 * ((samples + 1) * i + j + 0) + 1;		sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 1] = 5 * ((samples + 1) * i + j + 0) + 2;		sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 2] = 5 * ((samples + 1) * i + j + 1) + 2;
			sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 3] = 5 * ((samples + 1) * i + j + 1) + 2;		sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 4] = 5 * ((samples + 1) * i + j + 1) + 1;		sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 5] = 5 * ((samples + 1) * i + j + 0) + 1;
			sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 6] = 5 * ((samples + 1) * (j + 0) + i) + 3;	sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 7] = 5 * ((samples + 1) * (j + 0) + i) + 4;	sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 8] = 5 * ((samples + 1) * (j + 1) + i) + 4;
			sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 9] = 5 * ((samples + 1) * (j + 1) + i) + 4;	sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 10] = 5 * ((samples + 1) * (j + 1) + i) + 3;	sphereIndex[6 * samples * samples + 12 * (samples * i + j) + 11] = 5 * ((samples + 1) * (j + 0) + i + 0) + 3;
		}
	}

	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples; j++) {
			sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 0] = 5 * ((samples + 1) * (i + 0) + j + 0) + 1;	sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 1] = 5 * ((samples + 1) * (i + 1) + j + 1) + 4;	sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 2] = 5 * ((samples + 1) * (i + 1) + j + 1) + 2;
			sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 3] = 5 * ((samples + 1) * (i + 1) + j + 1) + 2;	sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 4] = 5 * ((samples + 1) * (i + 0) + j + 0) + 3;	sphereIndex[6 * samples * (3 * samples + 2) + 6 * (samples * i + j) + 5] = 5 * ((samples + 1) * (i + 0) + j + 0) + 1;
		}
	}
	*/

	return surface;
}

int Surface::VBOsize()
{
	return pow(m_samples + 1, 2);
}

int Surface::IBOsize()
{
	return 6 * pow(m_samples, 2);
}
