#include "stdafx.h"

// General
#include "CameraComponent.h"

CCameraComponent::CCameraComponent(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
	, m_RightDirection(0)
	, m_UpDirection(glm::vec3(0.0f, 1.0f, 0.0f))
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_View_Dirty(true)
{
	GetProperties()->SetName("CameraComponent");

	m_Yaw_XProperty = MakeShared(CPropertyWrapped<float>, "Yaw", "Rotation around the Y axis (in degrees).", 225.0f);
	m_Yaw_XProperty->SetValueSetter(std::bind(&CCameraComponent::SetYaw, this, std::placeholders::_1));
	m_Yaw_XProperty->SetValueGetter(std::bind(&CCameraComponent::GetYaw, this));
	GetProperties()->AddProperty(m_Yaw_XProperty);

	m_Pitch_YProperty = MakeShared(CPropertyWrapped<float>, "Pitch", "Rotation around the X and Y axis (in degrees).", -45.0f);
	m_Pitch_YProperty->SetValueSetter(std::bind(&CCameraComponent::SetPitch, this, std::placeholders::_1));
	m_Pitch_YProperty->SetValueGetter(std::bind(&CCameraComponent::GetPitch, this));
	GetProperties()->AddProperty(m_Pitch_YProperty);
}

CCameraComponent::~CCameraComponent()
{

}



//
// ICameraComponent3D
//
void CCameraComponent::DoMoveFront(float Value)
{
	if (Value == 0.0f)
		return;
	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetPosition(sceneNode3D.GetPosition() + sceneNode3D.GetLocalRotationDirection() * Value);
	m_View_Dirty = true;
}

void CCameraComponent::DoMoveBack(float Value)
{
	if (Value == 0.0f)
		return;
	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetPosition(sceneNode3D.GetPosition() - sceneNode3D.GetLocalRotationDirection() * Value);
	m_View_Dirty = true;
}

void CCameraComponent::DoMoveLeft(float Value)
{
	if (Value == 0.0f)
		return;
	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetPosition(sceneNode3D.GetPosition() - m_RightDirection * Value);
	m_View_Dirty = true;
}

void CCameraComponent::DoMoveRight(float Value)
{
	if (Value == 0.0f)
		return;
	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetPosition(sceneNode3D.GetPosition() + m_RightDirection * Value);
	m_View_Dirty = true;
}

void CCameraComponent::SetPosition(glm::vec3 Position)
{
	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetPosition(Position);
	m_View_Dirty = true;
}

glm::vec3 CCameraComponent::GetPosition() const
{
	return GetOwnerNode().GetPosition();
}

void CCameraComponent::SetDirection(glm::vec3 Direction)
{
	throw CException("Not implemented.");
	//ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	//sceneNode3D.SetLocalRotationEuler(Direction);
	//m_View_Dirty = true;
}

glm::vec3 CCameraComponent::GetDirection() const
{
	return GetOwnerNode().GetLocalRotationDirection();
}

void CCameraComponent::SetYaw(float Yaw)
{
	m_Yaw_X = Yaw;
	if (m_Yaw_X > 360.0f)
		m_Yaw_X -= 360.0f;
	if (m_Yaw_X < 0.0f)
		m_Yaw_X += 360.0f;

	m_Yaw_XProperty->RaiseValueChangedCallback();

	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetLocalRotationEuler(glm::vec3(m_Yaw_X, m_Pitch_Y, 0.0f));

	m_View_Dirty = true;
}

void CCameraComponent::AddYaw(float Yaw)
{
	SetYaw(m_Yaw_X + Yaw);
}

float CCameraComponent::GetYaw() const
{
	return m_Yaw_X;
}

void CCameraComponent::SetPitch(float Pitch)
{
	m_Pitch_Y = Pitch;
	if (m_Pitch_Y > cPitchUpperBorder)
		m_Pitch_Y = cPitchUpperBorder;
	if (m_Pitch_Y < cPitchBottomBorder)
		m_Pitch_Y = cPitchBottomBorder;

	m_Pitch_YProperty->RaiseValueChangedCallback();

	ISceneNode& sceneNode3D = const_cast<ISceneNode&>(GetOwnerNode());
	sceneNode3D.SetLocalRotationEuler(glm::vec3(m_Yaw_X, m_Pitch_Y, 0.0f));

	m_View_Dirty = true;
}

void CCameraComponent::AddPitch(float Pitch)
{
	SetPitch(m_Pitch_Y + Pitch);
}

float CCameraComponent::GetPitch() const
{
	return m_Pitch_Y;
}

void CCameraComponent::SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar)
{
	m_Perspective_FOV = fovy;

	m_Aspect = aspect;
	m_Near = zNear;
	m_Far = zFar;

	m_Projection = glm::perspectiveRH_ZO(glm::radians(fovy), aspect, zNear, zFar);
	m_Inverse_Projection = glm::inverse(m_Projection);
}

void CCameraComponent::SetOrthographicProjection(float aspect, float left, float right, float top, float bottom, float zNear, float zFar)
{
	m_Orthographic_Left = left;
	m_Orthographic_Right = right;
	m_Orthographic_Top = top;
	m_Orthographic_Bottom = bottom;

	m_Aspect = aspect;

	m_Near = zNear;
	m_Far = zFar;


	const float t = 50.0f;
	m_Projection = glm::orthoRH_ZO<float>(-t, t, -t, t, 0.0f, 100.0f);

	//m_Projection = glm::orthoRH_NO<float>(m_Orthographic_Left, m_Orthographic_Right, (m_Orthographic_Bottom / m_Aspect), (m_Orthographic_Top / m_Aspect), m_Near, m_Far);
	
	m_Inverse_Projection = glm::inverse(m_Projection);
}

const glm::mat4& CCameraComponent::GetViewMatrix() const
{
	const_cast<CCameraComponent*>(this)->UpdateView();
	return m_View;
}

const glm::mat4 & CCameraComponent::GetInverseViewMatrix() const
{
	const_cast<CCameraComponent*>(this)->UpdateView();
	return m_Inverse_View;
}

const glm::mat4& CCameraComponent::GetProjectionMatrix() const
{
	return m_Projection;
}

const glm::mat4& CCameraComponent::GetInverseProjectionMatrix() const
{
	return m_Inverse_Projection;
}

const Frustum & CCameraComponent::GetFrustum() const
{
	return m_Frustum;
}

const glm::vec3 & CCameraComponent::GetCameraUpDirection() const
{
	return m_UpDirection;
}



//
// ISceneNodeComponent
//
void CCameraComponent::OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message)
{
	if (Message == UUID_OnWorldTransformChanged)
	{
		UpdateView();
	}
}



//
// Protected
//
/*glm::vec3 CCameraComponent::AnglesToRotation(float Yaw, float Pitch)
{
	// Calculate the new Front vector
	glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
	direction.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)); // y
	direction.y = glm::sin(glm::radians(Pitch));                               // z
	direction.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)); // x
	direction = glm::normalize(direction);

	// Also re-calculate the Right and Up vector
	m_RightDirection = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_UpDirection = glm::cross(m_RightDirection, direction);

	return direction;
}

void CCameraComponent::RotationToAngles(const glm::vec3& Direction)
{
	// https://gamedev.stackexchange.com/questions/172147/convert-3d-direction-vectors-to-yaw-pitch-roll-angles

	// Yaw is the bearing of the forward vector's shadow in the xy plane.
	float yaw = atan2(GetOwnerNode().GetRotationEuler().z, GetOwnerNode().GetRotationEuler().x);

	// Pitch is the altitude of the forward vector off the xy plane, toward the down direction.
	float pitch = asin(GetOwnerNode().GetRotationEuler().y);

	// Find the vector in the xy plane 90 degrees to the right of our bearing.
	float planeRightX = glm::sin(yaw);
	float planeRightZ = -glm::cos(yaw);

	// Roll is the rightward lean of our up vector, computed here using a dot product.
	float roll = asin(m_UpDirection.x * planeRightX + m_UpDirection.z * planeRightZ);

	// If we're twisted upside-down, return a roll in the range +-(pi/2, pi)
	if (m_UpDirection.y < 0)
		roll = glm::sign(roll) * glm::pi<float>() - roll;


	float yawDegrees = glm::degrees(yaw);
	if (yawDegrees > 360.0f)
		yawDegrees -= 360.0f;
	if (yawDegrees < 0.0f)
		yawDegrees += 360.0f;

	float pitchDegress = glm::degrees(pitch);
	if (pitchDegress > cPitchUpperBorder)
		pitchDegress = cPitchUpperBorder;
	if (pitchDegress < cPitchBottomBorder)
		pitchDegress = cPitchBottomBorder;

	if (glm::abs(m_Yaw_X - yawDegrees) > 1.0f)
		_ASSERT_EXPR(false, "CCameraComponent: Error m_Yaw_X");

	if (glm::abs(m_Pitch_Y - pitchDegress) > 1.0f)
		_ASSERT_EXPR(false, "CCameraComponent: Error m_Pitch_Y");

	//SetYaw(m_Yaw_X2);
	//SetPitch(m_Pitch_Y2);
}*/

void CCameraComponent::UpdateView()
{
	if (false == m_View_Dirty)
		return;

	// Also re-calculate the Right and Up vector
	m_RightDirection = glm::normalize(glm::cross(GetOwnerNode().GetLocalRotationDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_UpDirection = glm::cross(m_RightDirection, GetOwnerNode().GetLocalRotationDirection());

	//m_View = glm::lookAt(-GetOwnerNode().GetRotationEuler() * 20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_View = glm::lookAt(glm::vec3(0.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_View = glm::lookAt(GetOwnerNode().GetPosition(), GetOwnerNode().GetPosition() + GetOwnerNode().GetLocalRotationDirection(), m_UpDirection);
	m_Inverse_View = glm::inverse(m_View);

	m_Frustum.buildViewFrustum(m_View, m_Projection);

	m_View_Dirty = false;
}
