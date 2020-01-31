#include "Pipeline.h"


void Pipeline::Scale(float s)
{
	Scale(s, s, s);
}

void Pipeline::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	m_scale.x = ScaleX;
	m_scale.y = ScaleY;
	m_scale.z = ScaleZ;
}

void Pipeline::Scale(vec3 scale) {
	m_scale = scale;
}

void Pipeline::WorldPos(float x, float y, float z)
{
	m_worldPos.x = x;
	m_worldPos.y = y;
	m_worldPos.z = z;
}

void Pipeline::WorldPos(vec3 XYZ)
{
	m_worldPos = XYZ;
}

void Pipeline::Rotate(float RotateX, float RotateY, float RotateZ)
{
	m_rotateInfo.x = RotateX;
	m_rotateInfo.y = RotateY;
	m_rotateInfo.z = RotateZ;
}

void Pipeline::Rotate(vec3 RotateXYZ)
{
	m_rotateInfo = RotateXYZ;
}

void Pipeline::setPersProj(const PersProjInfo& p)
{
	m_persProj = p;
}

void Pipeline::setCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp)
{
	m_camera.Pos = cameraPos;
	m_camera.Target = cameraTarget;
	m_camera.Up = cameraUp;
}

void Pipeline::setCamera(const Camera& camera)
{
	setCamera(camera.GetPos(), camera.GetTarget(), camera.GetUp());
}

const mat4* Pipeline::getWVPTrans()
{
	mat4 ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjection;

	ScaleTrans = initScaleTrans(m_scale);
	RotateTrans = initRotateTrans(m_rotateInfo);
	TranslationTrans = initTranslationTrans(m_worldPos);
	CameraTranslationTrans = initTranslationTrans(-m_camera.Pos);
	CameraRotateTrans = initCameraTransform(m_camera.Target, m_camera.Up);
	PersProjection = initPersProjection(m_persProj);

	m_transformation = PersProjection * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;
}

const mat4* Pipeline::getXAxisRot()
{
	mat4 RotateTrans;
	vec3 Normal = normalize(vec3(0.0, m_camera.Pos.y, m_camera.Pos.z));
	
	RotateTrans = initRotateTrans(vec3(-180 * atan(Normal.y / Normal.z) / pi<float>(), 0.0f, 0.0f));

	m_xTransformation = RotateTrans;

	return &m_xTransformation;
}

const mat4* Pipeline::getYAxisRot()
{
	mat4 RotateTrans;
	vec3 Normal = normalize(vec3(m_camera.Pos.x, 0.0, m_camera.Pos.z));

	RotateTrans = initRotateTrans(vec3(0.0, 180 * atan(Normal.z / Normal.x) / pi<float>(), 0.0f));

	m_yTransformation = RotateTrans;

	return &m_yTransformation;
}

const mat4* Pipeline::getZAxisRot()
{
	mat4 RotateTrans;
	vec3 Normal = normalize(vec3(m_camera.Pos.x, m_camera.Pos.y, 0.0f));

	RotateTrans = initRotateTrans(vec3(0.0, 0.0f, -180 * atan(Normal.x / Normal.y) / pi<float>()));

	m_zTransformation = RotateTrans;

	return &m_zTransformation;
}

