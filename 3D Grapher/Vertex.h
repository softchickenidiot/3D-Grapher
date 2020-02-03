#pragma once

#include "glm.hpp"

using namespace glm; 

struct Vertex
{
	vec3 m_pos;
	vec4 m_color;
	int m_shader;

	Vertex() {}

	Vertex(vec3 pos, vec4 color, int shader)
	{
		m_pos = pos;
		m_color = color;
		m_shader = shader;
	}
};