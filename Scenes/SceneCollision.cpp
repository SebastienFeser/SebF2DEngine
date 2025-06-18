#include "SceneCollision.h"
#include "../Physics/PhysicsConstants.h"

void SceneCollision::Init()
{
	auto circle1 = m_entityManager.AddEntity("Circle");
	circle1->cTransform = std::make_shared<CTransform>(Vec2(1, 1));
	circle1->cShape = std::make_shared<CCircle>(50.0f);
	circle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	circle1->cRigidbody->m_velocity = Vec2(3, 0);

	auto circle2 = m_entityManager.AddEntity("Circle");
	circle2->cTransform = std::make_shared<CTransform>(Vec2(5, 1));
	circle2->cShape = std::make_shared<CCircle>(50.0f);
	circle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	circle2->cRigidbody->m_velocity = Vec2(-3, 0);
}

void SceneCollision::Update(float dt)
{
	m_entityManager.Update();
	m_physics.Update(m_entityManager, dt);
}

void SceneCollision::Render(sf::RenderWindow& m_window)
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

	m_window.display();
}

void SceneCollision::ProcessInput(sf::RenderWindow& m_window)
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				m_game->ChangeScene("SceneGravity");
			}
		}
	}
}

void SceneCollision::OnEnd()
{

}