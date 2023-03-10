#pragma once
#include "../GameObject.h"
#include "../Components/QuadRenderer.h"
#include "../Components/Movement.h"

class Pad : public GameObject
{
public:
	explicit Pad(float xPosition, std::shared_ptr<D3D11Shader> shader)
		: GameObject(std::make_shared<QuadRenderer>(shader)),
		m_Movement(std::make_shared<Movement>(m_Transform))
	{
		m_Transform->SetLocalScale(DirectX::SimpleMath::Vector3{ 0.01f, 0.2f, 1.0f });
		m_Transform->SetPositionX(xPosition);
	}

	~Pad() override = default;
	void Update(float deltaTime) override
	{
		GameObject::Update(deltaTime);

		if (ShouldGoDown())
			m_Movement->velocity = DirectX::SimpleMath::Vector3{ 0.0f, -c_Speed, 0.0f };
		else if (ShouldGoUp())
			m_Movement->velocity = DirectX::SimpleMath::Vector3{ 0.0f, c_Speed, 0.0f };
		else
			m_Movement->velocity = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 0.0f };

		m_Movement->Update(deltaTime);
	}

	DirectX::SimpleMath::Vector3 GetVelocity() const
	{
		return m_Movement->velocity;
	}

protected:
	virtual bool ShouldGoDown() = 0;
	virtual bool ShouldGoUp() = 0;

	const float c_Speed = 1.0f;

	std::shared_ptr<Movement> m_Movement;
};
