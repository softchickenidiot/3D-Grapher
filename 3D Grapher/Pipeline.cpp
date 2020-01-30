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

const mat4* Pipeline::getXCamRot()
{
	mat4 RotateTrans;
	vec3 Normal = normalize(vec3(0.0, m_camera.Target.y, m_camera.Target.z));
	std::cout << '\n' << "< 0.0, " << Normal.y << ", " << Normal.z << " >\n" << 180 * asin(Normal.y) / pi<float>();
	
	RotateTrans = initRotateTrans(vec3(180 * asin(Normal.y) / pi<float>(), 0.0f, 0.0f));

	m_xTransformation = RotateTrans;

	return &m_xTransformation;
}

