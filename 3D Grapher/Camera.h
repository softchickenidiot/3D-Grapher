#pragma once

#include <iostream>
#include "GL/freeglut.h"
#include "glm.hpp"
#include "gtc/constants.hpp"
#include "gtx/rotate_vector.hpp"

using namespace glm;

class Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidth, int WindowHeight, const vec3 Pos, const vec3 Target, const vec3 Up);

	bool OnKeyboard(unsigned char Key);
	void OnMouse(int x, int y);
	void OnRender();

	vec3 GetPos() const;
	vec3 GetTarget() const;
	vec3 GetUp() const;

private:
	int m_WindowWidth;
	int m_WindowHeight;

	vec3 m_pos;
	vec3 m_target;
	vec3 m_up;

	float m_AngleH;
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	ivec2 m_mousePos;

	void init();
	void update();
};
