#include "VectorFunction.h"

vec3 VectorFunction::PositionFunc(float t)
{
	m_r = {Function(m_xFunc, t), Function(m_yFunc, t), Function(m_zFunc, t) };

	return m_r;
}

vec3 VectorFunction::TangentFunc(float t)
{		              
	m_T = (PositionFunc(t + m_h) - PositionFunc(t - m_h)) / m_h;
	m_T /= 2.0f;
	m_T /= sqrt(m_T.x * m_T.x + m_T.y * m_T.y + m_T.z * m_T.z);
	return m_T;
}

vec3 VectorFunction::NormalFunc(float t)
{                    
	m_N = (TangentFunc(t + m_h) - TangentFunc(t - m_h)) / m_h;
	m_N /= 2.0f;

	if (m_N.x == 0.0f && m_N.y == 0.0f && m_N.z == 0.0f) {
		m_N = vec3(1.0f, -1.0f, 1.0f);
	}
	m_N /= sqrt(m_N.x * m_N.x + m_N.y * m_N.y + m_N.z * m_N.z);
	cout << m_N.x;
	return m_N;
}

float VectorFunction::Function(const char* func, float t)
{
	m_t = 0.0f;

	for (int i = 0; i < strlen(func); i++) 
	{
		if (func[i] == 't') {
			if (func[i + 1] == '^') {
				m_t += pow(t, int(func[i + 2]) - 48);
			}
		}
	}
	return m_t;
}
