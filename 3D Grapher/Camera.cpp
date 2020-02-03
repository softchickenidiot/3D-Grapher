#include "Camera.h"

const static float K_STEP_SCALE = 0.1f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight)
{
	m_WindowWidth = WindowWidth;
	m_WindowHeight = WindowHeight;

	m_pos = vec3(0.0f, 0.0f, 0.0f);
	m_target = vec3(0.0f, 0.0f, 1.0f);
	m_up = vec3(0.0f, 1.0f, 0.0f);

	init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const vec3 Pos, const vec3 Target, const vec3 Up)
{
	m_WindowWidth = WindowWidth;
	m_WindowHeight = WindowHeight;

	m_pos = Pos;

	m_target = Target;
	m_target = normalize(m_target);

	m_up = Up;
	m_up = normalize(m_up);

	init();
}


bool Camera::OnKeyboard(unsigned char Key)
{
	bool Ret = false;

	switch (Key) {

	case 'w':
	{
		m_pos += (m_target * K_STEP_SCALE);
		Ret = true;
	}
	break;

	case 's':
	{
		m_pos -= (m_target * K_STEP_SCALE);
		Ret = true;
	}
	break;

	case 'a':
	{
		vec3 Left = cross(m_target, m_up);
		Left = normalize(Left);
		Left *= K_STEP_SCALE;
		m_pos += Left;
		Ret = true;
	}
	break;

	case 'd':
	{
		vec3 Right = cross(m_up, m_target);
		Right = normalize(Right);
		Right *= K_STEP_SCALE;
		m_pos += Right;
		Ret = true;
	}
	break;
	}

	return Ret;
}

void Camera::OnMouse(int x, int y)
{
	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	m_AngleH += (float)DeltaX / 160.0f;
	m_AngleV += (float)DeltaY / 160.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			m_OnLeftEdge = true;
		}
		else if (x >= (m_WindowWidth - MARGIN)) {
			m_OnRightEdge = true;
		}
	}
	else {
		m_OnLeftEdge = false;
		m_OnRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			m_OnUpperEdge = true;
		}
		else if (y >= (m_WindowHeight - MARGIN)) {
			m_OnLowerEdge = true;
		}
	}
	else {
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}

	update();
}

void Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (m_OnLeftEdge) {
		m_AngleH -= 0.01f;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += 0.01f;
		ShouldUpdate = true;
	}

	if (m_OnUpperEdge) {
		if (m_AngleV > -90.0f) {
			m_AngleV -= 0.01f;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge) {
		if (m_AngleV < 90.0f) {
			m_AngleV += 0.01f;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		update();
	}
}


vec3 Camera::GetPos() const
{
	return m_pos;
}

vec3 Camera::GetTarget() const
{
	return m_target;
}

vec3 Camera::GetUp() const
{
	return m_up;
}


void Camera::init()
{
	vec3 HTarget(m_target.x, 0.0f, m_target.z);
	HTarget = normalize(HTarget);

	if (HTarget.z >= 0.0f) {
		if (HTarget.x >= 0.0f) {
			m_AngleH = 360.0f - (asin(HTarget.z) * 180.0f / pi<float>());
		}
		else {
			m_AngleH = 180.0f + (asin(HTarget.z) * 180.0f / pi<float>());
		}
	}
	else {
		if (HTarget.x >= 0.0f) {
			m_AngleH = (asin(-HTarget.z) * 180.0f / pi<float>());
		}
		else {
			m_AngleH = 180.0f - (asin(-HTarget.z) * 180.0f / pi<float>());
		}
	}
	m_AngleV = -(asin(m_target.y) * 180.0f / pi<float>());

	m_OnUpperEdge = false;
	m_OnLowerEdge = false;
	m_OnLeftEdge = false;
	m_OnRightEdge = false;
	m_mousePos.x = m_WindowWidth / 2;
	m_mousePos.y = m_WindowHeight / 2;

	glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

void Camera::update()
{
	const vec3 Vaxis(0.0f, 1.0f, 0.0f);

	vec3 View(1.0f, 0.0f, 0.0f);
	View = rotate(View, m_AngleH, Vaxis);
	View = normalize(View);

	vec3 Haxis = cross(View, Vaxis);
	Haxis = normalize(Haxis);
	View = rotate(View, m_AngleV, Haxis);
	View = normalize(View);

	m_target = View;
	m_target = normalize(m_target);

	m_up = cross(m_target, Haxis);
	m_up = normalize(m_up);
}