#include "SceneFalling.h"
#include "../Physics/PhysicsConstants.h"

void SceneFalling::Init()
{
	auto circle1 = m_entityManager.AddEntity("Circle");
	circle1->cTransform = std::make_shared<CTransform>(Vec2(1, 1));
	circle1->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle1->cRigidbody->m_velocity = Vec2(0, 0);
	circle1->cRigidbody->m_bounce = 1.0f;
	circle1->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle1->cCollisionState = std::make_shared<CCollisionState>();

	auto rectangle1 = m_entityManager.AddEntity("OBB");
	rectangle1->cTransform = std::make_shared<CTransform>(Vec2(3, 1), 30);
	rectangle1->cShape = std::make_shared<COBB>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER), 30);
	rectangle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle1->cRigidbody->m_velocity = Vec2(0, 0);
	auto collider1 = std::make_shared<COBBCollider>(Vec2(0.5f, 0.5f), 30);
	rectangle1->cCollider = collider1;
	rectangle1->cCollisionState = std::make_shared<CCollisionState>();
	rectangle1->cRigidbody->SetMassAndInertia(1.0, collider1->GetMomentOfInertia());

	auto ground = m_entityManager.AddEntity("OBB");
	ground->cTransform = std::make_shared<CTransform>(Vec2(5, 5), 0);
	ground->cShape = std::make_shared<COBB>(Vec2(10.0f * PIXELS_PER_METER, 3.0f * PIXELS_PER_METER), 0);
	ground->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::STATIC);
	ground->cRigidbody->m_velocity = Vec2(0, 0);
	auto collider2 = std::make_shared<COBBCollider>(Vec2(10.0f, 3.0f), 0);
	ground->cCollider = collider2;
	ground->cCollisionState = std::make_shared<CCollisionState>();
	ground->cRigidbody->SetMassAndInertia(100.0, collider2->GetMomentOfInertia());

	m_physics.m_useQuadTree = false;
}

void SceneFalling::Update(float dt)
{
	m_entityManager.Update();
	m_physics.Update(m_entityManager, dt);
}

void SceneFalling::Render(sf::RenderWindow& m_window)
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
			shape->setRotation(sf::degrees(e->cTransform->m_angle));
			m_window.draw(*shape);
		}
	}

	//m_window.display();
}

void SceneFalling::ProcessInput(sf::RenderWindow& m_window)
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

void SceneFalling::OnEnd()
{

}