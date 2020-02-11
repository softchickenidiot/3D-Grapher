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
	return m_N;
}

float VectorFunction::Function(string func, float t)
{
	//Format and input t
	for (size_t i = 0; i < func.length(); i++) 
	{
		switch (func[i]) {
		case ' ': 
			func.erase(func.begin() + i);
			i--;
			break;
		case 't':
			func.replace(i, 1, to_string(t));
			i--;
			break;
		case '.': 
			if (!isdigit(func[i - 1]) || !isdigit(func[i + 1])) {
				cout << "Syntax ERROR\n";
				exit(0);
			}
			break;
		default: 
			break;
		}
	}
	//Parentheses
	for (size_t i = 0; i < func.length(); i++) {
		if (func[i] == ')') {
			for (size_t j = i; j >= 0; j--) {
				if (func[j] == '(') {
					func.replace(j, i - j + 1, to_string(Function(func.substr(j + 1, i - j - 1), t)));
					i = j;
					break;
				}
				else if (j == 0) {
					cout << "Syntax ERROR\n";
					exit(0);
				}
			}

		}
	}
	//Exponents
	for (size_t i = 0; i < func.length(); i++) {
		if (func[i] == '^') {
			if (!(isdigit(func[i - 1]) || func[i - 1] == '.') || !(isdigit(func[i + 1]) || func[i + 1] == '.' || func[i + 1] == '-')) {
				cout << "Syntax ERROR\n";
				exit(0);
			}
			for (size_t j = i - 1; j >= 0; j--) {
				if (func[j] == '-' ) {
					func.erase(j, i - j);
					i -= i - j;
					break;
				}
				else if (!isdigit(func[j]) && func[j] != '.') {
					m_c = stof(func.substr(j + 1, i - j - 1));
					func.erase(j + 1, i - j - 1);
					i -= i - j - 1;
					break;
				}
				else if (j == 0) {
					m_c = stof(func.substr(j, i - j));
					func.erase(j, i - j);
					i -= i - j;
					break;
				}
			}
			for (size_t j = i + 1; j < func.length(); j++) {
				if (func[j] == '-' && j == i + 1) {
					continue;
				}
				else if (!isdigit(func[j]) && func[j] != '.') {
					func.replace(i, j - i, to_string(powf(m_c, stof(func.substr(i + 1, j - i - 1)))));
					break;
				}
				else if (j == func.length() - 1) {
					func.replace(i, j - i + 1, to_string(powf(m_c, stof(func.substr(i + 1, j - i)))));
					break;
				}
			}
		}
	}

	cout << func << '\n';
	return stof(func);
}
