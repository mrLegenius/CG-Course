#include "OrbitCamera.h"

#include <iostream>

#include "../Input/Input.h"

OrbitCamera::OrbitCamera(float fov, float nearClip, float farClip)
{
	SetPerspective(fov, nearClip, farClip);
	UpdateView();
}

void OrbitCamera::RotateAzimuth(const float radians)
{
	m_AzimuthAngle += radians;

	// Keep azimuth angle within range <0..2PI) - it's not necessary, just to have it nicely output
	constexpr auto fullCircle = DirectX::XM_2PI;
	m_AzimuthAngle = fmodf(m_AzimuthAngle, fullCircle);
	if (m_AzimuthAngle < 0.0f) {
		m_AzimuthAngle = fullCircle + m_AzimuthAngle;
	}
}

void OrbitCamera::RotatePolar(const float radians)
{
	m_PolarAngle += radians;

	constexpr auto polarCap = DirectX::XM_PIDIV2 - 0.001f;
	if (m_PolarAngle > polarCap) {
		m_PolarAngle = polarCap;
	}

	if (m_PolarAngle < -polarCap) {
		m_PolarAngle = -polarCap;
	}
}

void OrbitCamera::Zoom(const float value)
{
	m_Distance += value;
	if (m_Distance < 10) {
		m_Distance = 10;
	}
}

DirectX::SimpleMath::Vector3 OrbitCamera::CalculatePosition() const
{
	// Calculate sines / cosines of angles
	const auto sineAzimuth = sin(m_AzimuthAngle);
	const auto cosineAzimuth = cos(m_AzimuthAngle);
	const auto sinePolar = sin(m_PolarAngle);
	const auto cosinePolar = cos(m_PolarAngle);

	// Calculate eye position out of them
	auto x = m_Distance * cosinePolar * cosineAzimuth;
	auto y = m_Distance * sinePolar;
	auto z = m_Distance * cosinePolar * sineAzimuth;

	if (m_Target)
	{
		x += m_Target->position.x;
		y += m_Target->position.y;
		z += m_Target->position.z;
	}

	return { x, y, z };
}

void OrbitCamera::UpdateView()
{
	const auto targetPosition = m_Target ? m_Target->position : DirectX::SimpleMath::Vector3::Zero;
		
	if (m_Transform.position == targetPosition) return;

	m_ViewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_Transform.position, targetPosition, DirectX::SimpleMath::Vector3::Up);
}

void OrbitCamera::Update(const float deltaTime)
{
	constexpr auto ZoomSensitivity = 10;
	constexpr auto rotationSpeed = 1;

	const auto [mouseDeltaX, mouseDeltaY] = Input::GetMouseDelta();

	const auto deltaX = mouseDeltaX * rotationSpeed * deltaTime;
	const auto deltaY = mouseDeltaY * rotationSpeed * deltaTime;

	Zoom(deltaTime * ZoomSensitivity * -Input::GetMouseWheelOffset());

	RotatePolar(deltaY);
	RotateAzimuth(deltaX);

	m_Transform.position = CalculatePosition();

	UpdateView();
}
