#include "SceneGravity.h"
#include "../Physics/PhysicsConstants.h"

void SceneGravity::Init()
{
	auto circle = m_entityManager.AddEntity("Circle");
	circle->cTransform = std::make_shared <CTransform>(Vec2(1, 1));
	circle->cShape = std::make_shared<CCircle>(50.0f);
	circle->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
}

void SceneGravity::Update(float dt)
{
	m_entityManager.Update();
	m_physics.Update(m_entityManager, dt);
	//Handle Inputs
	//Handle Movement
	//Handle Collisions
}

void SceneGravity::Render(sf::RenderWindow& m_window)
{
	m_window.clear();

	for (auto& e : m_entityManager.GetEntities())
	{
		if (e->cShape && e->cTransform)
		{
			auto shape = e->cShape->CreateSFShape();
			shape->setPosition(sf::Vector2f(e->cTransform->m_position.x * PIXELS_PER_METER, e->cTransform->m_position.y * PIXELS_PER_METER));
			m_window.draw(*shape);
		}
	}

	//m_window.display();
}

void SceneGravity::ProcessInput(sf::RenderWindow& m_window)
{
	while (const std::optional event = m_window.pollEvent())
	{

		if (event->is<sf::Event::Closed>())
			m_window.close();
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				m_game->ChangeScene("SceneCollision");
			}
		}
	}
}

void SceneGravity::OnEnd()
{

}