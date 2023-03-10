#pragma once

#include "Camera.h"
#include "../../Components/Transform.h"
#include "directxmath.h"

class SceneCamera : public Camera
{
public:
	enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	SceneCamera();
	~SceneCamera() override = default;
	void SetViewportSize(uint32_t width, uint32_t height);

	void SetProjectionType(const ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
	[[nodiscard]] ProjectionType GetProjectionType() const { return m_ProjectionType; }
	[[nodiscard]] const Matrix& GetViewMatrix() const { return m_ViewMatrix; }
	[[nodiscard]] Matrix GetViewProjection() const { return m_ViewMatrix * m_Projection; }

	[[nodiscard]] const Transform& GetTransform() const { return m_Transform; }
	void SetPosition(const Vector3& value) { m_Transform.SetWorldPosition(value); }
	[[nodiscard]] Vector3 GetPosition() const { return m_Transform.GetWorldPosition(); }
	virtual void Update(float deltaTime) = 0;

private:
	void RecalculateProjection();

	ProjectionType m_ProjectionType = ProjectionType::Perspective;
	float m_AspectRatio = 1.0f;

protected:
	Transform m_Transform;

	Matrix m_ViewMatrix;
	virtual void UpdateView();

	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Orthographic -------------------------------------------------------------------------------------------
	// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
			
public:
	void SetOrthographic(float size, float nearClip, float farClip);

	void SetOrthographicSize(const float size) { m_OrthographicSize = size; RecalculateProjection(); }
	[[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }

	void SetOrthographicNear(const float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
	[[nodiscard]] float GetOrthographicNear() const { return m_OrthographicNear; }

	void SetOrthographicFar(const float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
	[[nodiscard]] float GetOrthographicFar() const { return m_OrthographicFar; }
private:
	float m_OrthographicSize = 10.0f;
	float m_OrthographicNear = -1.0f;
	float m_OrthographicFar = 1.0f;

	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Perspective --------------------------------------------------------------------------------------------
	// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
			
public:
	void SetPerspective(float fov, float nearClip, float farClip);

	void SetFieldOfView(const float fov) { m_FieldOfView = DirectX::XMConvertToRadians(fov); RecalculateProjection(); }
	[[nodiscard]] float GetFieldOfView() const { return m_FieldOfView; }

	void SetPerspectiveNear(const float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
	[[nodiscard]] float GetPerspectiveNear() const { return m_PerspectiveNear; }

	void SetPerspectiveFar(const float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
	[[nodiscard]] float GetPerspectiveFar() const { return m_PerspectiveFar; }

private:
	float m_FieldOfView = DirectX::XMConvertToRadians(45.0f);
	float m_PerspectiveNear = 0.1f;
	float m_PerspectiveFar = 1000.0f;
};