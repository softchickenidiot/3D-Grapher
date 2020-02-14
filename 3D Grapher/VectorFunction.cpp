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
	//Format and Input t and constants
	for (size_t i = 0; i < func.length(); i++) 
	{
		switch (func[i]) {
		case ' ': 
			func.erase(func.begin() + i);
			i--;
			break;
		case 't':
			if (func[i + 1] != 'a') {
				func.replace(i, 1, to_string(t));
				i--;
			}
			break;
		case '.': 
			if (!isdigit(func[i - 1]) || !isdigit(func[i + 1])) {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			break;
		case 'p':
			if (func[i + 1] == 'i') {
				func.replace(i, 1, to_string(pi<float>()));
				i--;
			}
			else {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			break;
		case 'e':
			func.replace(i, 1, to_string(exp(1)));
			i--;
			break;
		default: 
			break;
		}
	}
	//Parentheses
	for (size_t i = 0; i < func.length(); i++)
	{
		if (func[i] == ')') {
			for (size_t j = i; j >= 0; j--) {
				if (func[j] == '(') {
					func.replace(j, i - j + 1, to_string(Function(func.substr(j + 1, i - j - 1), t)));
					i = j;
					break;
				}
				else if (j == 0) {
					cout << func << "\nSyntax Error\n";
					exit(0);
				}
			}

		}
	}
	//Trig and ln Functions 
	for (size_t i = 0; i < func.length(); i++) {
		switch (func[i]) {
		case 'c':
			if (func[i + 1] == 'o' && func[i + 2] == 's' && (isdigit(func[i + 3]) || func[i + 3] == '-' || func[i + 3] == '.')) {
				for (size_t j = i + 3; j < func.length(); j++) {
					if (func[j] == '-' && j == i + 3) {
						continue;
					}
					if (!isdigit(func[j]) && func[j] !='.') {
						func.replace(i, j - i, to_string(cosf(Function(func.substr(i + 3, j - i - 3), t))));
						i--;
						break;
					}
					else if (j == func.length() - 1) {
						func.replace(i, j - i + 1, to_string(cosf(Function(func.substr(i + 3, j - i - 2), t))));
						i--;
						break;
					}
				}
			}
			else {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			break;
		case 's':
			if (func[i + 1] == 'i' && func[i + 2] == 'n' && (isdigit(func[i + 3]) || func[i + 3] == '-' || func[i + 3] == '.')) {
				for (size_t j = i + 3; j < func.length(); j++) {
					if (func[j] == '-' && j == i + 3) {		
						continue;
					}
					if (!isdigit(func[j]) && func[j] != '.') {
						func.replace(i, j - i, to_string(sinf(Function(func.substr(i + 3, j - i - 3), t))));
						i--;
						break;
					}
					else if (j == func.length() - 1) {
						func.replace(i, j - i + 1, to_string(sinf(Function(func.substr(i + 3, j - i - 2), t))));
						i--;
						break;
					}
				}
			}
			else {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			break;
		case 't':
			if (func[i + 1] == 'a' && func[i + 2] == 'n' && (isdigit(func[i + 3]) || func[i + 3] == '-' || func[i + 3] == '.')) {
				for (size_t j = i + 3; j < func.length(); j++) {
					if (func[j] == '-' && j == i + 3) {
						continue;
					}
					if (!isdigit(func[j]) && func[j] != '.') {
						func.replace(i, j - i, to_string(tanf(Function(func.substr(i + 3, j - i - 3), t))));
						i--;
						break;
					}
					else if (j == func.length() - 1) {
						func.replace(i, j - i + 1, to_string(tanf(Function(func.substr(i + 3, j - i - 2), t))));
						i--;
						break;
					}
				}
			}
			else {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			break;
		default: 
			break;
		}
	}
	//Exponents
	for (size_t i = 0; i < func.length(); i++) 
	{
		if (func[i] == '^') {
			if (!(isdigit(func[i - 1]) || func[i - 1] == '.') || !(isdigit(func[i + 1]) || func[i + 1] == '.' || func[i + 1] == '-')) {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			for (size_t j = i - 1; j >= 0; j--) {
				if (func[j] == '-' ) {
					m_c = stof(func.substr(j, i - j));
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
	//Multiplication 
	for (size_t i = 0; i < func.length(); i++)
	{
		if (func[i] == '*') {
			if (!(isdigit(func[i - 1]) || func[i - 1] == '.') || !(isdigit(func[i + 1]) || func[i + 1] == '-' || func[i + 1] == '.')) {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			for (size_t j = i - 1; j >= 0; j--) {
				if (func[j] == '-') {
					m_c = stof(func.substr(j, i - j));
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
				if (!isdigit(func[j]) && func[j] != '.') {
					func.replace(i, j - i, to_string(m_c* stof(func.substr(i + 1, j - i - 1))));
					break;
				}
				else if (j == func.length() - 1) {
					func.replace(i, j - i + 1, to_string(m_c * stof(func.substr(i + 1, j - i))));
					break;
				}
			}
		}
	}
	//Division
	for (size_t i = 0; i < func.length(); i++)
	{
		if (func[i] == '/') {
			if (!(isdigit(func[i - 1]) || func[i - 1] == '.') || !(isdigit(func[i + 1]) || func[i + 1] == '-' || func[i + 1] == '.')) {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			for (size_t j = i - 1; j >= 0; j--) {
				if (func[j] == '-') {
					m_c = stof(func.substr(j, i - j));
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
				if (!isdigit(func[j]) && func[j] != '.') {
					func.replace(i, j - i, to_string(m_c / stof(func.substr(i + 1, j - i - 1))));
					break;
				}
				else if (j == func.length() - 1) {
					func.replace(i, j - i + 1, to_string(m_c / stof(func.substr(i + 1, j - i))));
					break;
				}
			}
		}
	}
	//Addition Format
	for (size_t i = 0; i < func.length(); i++)
	{
		if (func[i] == '+') {
			if (func[i - 1] == '-' || func[i + 1] == '+') {
				func.erase(i, 1);
				i = -1;
				continue;
			}
		}
		if (func[i] == '-') {
			if (func[i + 1] == '-') {
				func.replace(i, 2, "+");
				i = -1;
				continue;
			}
			if (func[i - 1] != '+') {
				func.insert(i, "-");
			}
		}
	}
	//Addition Operation 
	for (size_t i = 0; i < func.length(); i++)
	{
		if (func[i] == '+') {
			if (!(isdigit(func[i - 1]) || func[i - 1] == '.') || !(isdigit(func[i + 1]) || func[i + 1] == '-' || func[i + 1] == '.')) {
				cout << func << "\nSyntax Error\n";
				exit(0);
			}
			for (size_t j = i - 1; j >= 0; j--) {
				if (func[j] == '-') {
					m_c = stof(func.substr(j, i - j));
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
				if (!isdigit(func[j]) && func[j] != '.') {
					func.replace(i, j - i, to_string(m_c + stof(func.substr(i + 1, j - i - 1))));
					break;
				}
				else if (j == func.length() - 1) {
					func.replace(i, j - i + 1, to_string(m_c + stof(func.substr(i + 1, j - i))));
					break;
				}
			}
		}
	}
	
	return stof(func);
}
