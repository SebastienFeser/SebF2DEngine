#include "SceneFallingCircle.h"
#include "../Physics/PhysicsConstants.h"

void SceneFallingCircle::Init()
{
	auto circle1 = m_entityManager.AddEntity("Circle");
	circle1->cTransform = std::make_shared<CTransform>(Vec2(1.4, 1));
	circle1->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle1->cShape->SetTexture(&basketballTexture);
	circle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle1->cRigidbody->m_velocity = Vec2(0, 0);
	circle1->cRigidbody->m_bounce = 1.0f;
	circle1->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle1->cCollisionState = std::make_shared<CCollisionState>();
	circle1->cRigidbody->SetMassAndInertia(1.0f, circle1->cCollider->GetMomentOfInertia());

	auto circle2 = m_entityManager.AddEntity("Circle");
	circle2->cTransform = std::make_shared<CTransform>(Vec2(1, 5));
	circle2->cShape = std::make_shared<CCircle>(1.0 * PIXELS_PER_METER);
	circle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::STATIC);
	circle2->cRigidbody->m_velocity = Vec2(0, 0);
	circle2->cRigidbody->m_bounce = 1.0f;
	circle2->cCollider = std::make_shared<CCircleCollider>(1.0f);
	circle2->cCollisionState = std::make_shared<CCollisionState>();

	m_physics.m_useQuadTree = false;
}

void SceneFallingCircle::Update(float dt)
{
	m_entityManager.Update();
	m_physics.Update(m_entityManager, dt);
}

void SceneFallingCircle::Render(sf::RenderWindow& m_window)
{
	m_window.clear();

	for (auto& e : m_entityManager.GetEntities())
	{
		if (e->cShape && e->cTransform)
		{
			auto shape = e->cShape->CreateSFShape();
			if (e->cCollisionState && e->cCollisionState->isColliding)
			{
				shape->setFillColor(sf::Color::Green);
			}
			else
			{
				shape->setFillColor(sf::Color::Red);
			}
			shape->setPosition(sf::Vector2f(e->cTransform->m_position.x * PIXELS_PER_METER, e->cTransform->m_position.y * PIXELS_PER_METER));
			shape->setRotation(sf::radians(e->cTransform->m_angle));
			m_window.draw(*shape);
		}
	}

	//m_window.display();
}

void SceneFallingCircle::ProcessInput(sf::RenderWindow& m_window)
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

void SceneFallingCircle::OnEnd()
{

}