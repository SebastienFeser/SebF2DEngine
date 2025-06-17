#include "SceneGravity.h"

void SceneGravity::Init()
{
	auto circle = m_entityManager.AddEntity("Circle");
	circle->cTransform = std::make_shared <CTransform>(Vec2(100, 100));
	circle->cShape = std::make_shared<CCircle>(50.0f);
	circle->cRigidbody = std::make_shared<CRigidbody>(1.0f, false);
	circle->cRigidbody->m_velocity = Vec2(5, 5);
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
			shape->setPosition(sf::Vector2f(e->cTransform->m_position.x, e->cTransform->m_position.y));
			m_window.draw(*shape);
		}
	}

	m_window.display();
}

void SceneGravity::ProcessInput(sf::RenderWindow& m_window)
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				m_game->ChangeScene("SceneGravity");
			}
		}
	}
}

void SceneGravity::OnEnd()
{

}