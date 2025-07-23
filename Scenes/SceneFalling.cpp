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
	const std::vector<Vec2> fallingRectPoints = {
	Vec2(-1.0f, -0.5f),
	Vec2(1.0f, -0.5f),
	Vec2(1.0f,  0.5f),
	Vec2(-1.0f,  0.5f)
	};
	const std::vector<Vec2> fallingRectPointsPixeled = {
	Vec2(-1.0f * PIXELS_PER_METER, -0.5f * PIXELS_PER_METER),
	Vec2(1.0f * PIXELS_PER_METER, -0.5f * PIXELS_PER_METER),
	Vec2(1.0f * PIXELS_PER_METER,  0.5f * PIXELS_PER_METER),
	Vec2(-1.0f * PIXELS_PER_METER,  0.5f * PIXELS_PER_METER)
	};
	rectangle1->cTransform = std::make_shared<CTransform>(Vec2(3, 1), 3);
	rectangle1->cShape = std::make_shared<CPolygon>(fallingRectPointsPixeled);
	rectangle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle1->cRigidbody->m_velocity = Vec2(0, 0);
	auto collider1 = std::make_shared<CPolygonCollider>(fallingRectPoints);
	rectangle1->cCollider = collider1;
	rectangle1->cCollisionState = std::make_shared<CCollisionState>();
	rectangle1->cRigidbody->SetMassAndInertia(1.0, collider1->GetMomentOfInertia());

	auto ground = m_entityManager.AddEntity("OBB");
	const std::vector<Vec2> groundPoints = {
	Vec2(-5.0f, -0.5f),
	Vec2(5.0f, -0.5f),
	Vec2(5.0f,  0.5f),
	Vec2(-5.0f,  0.5f)
	};
	const std::vector<Vec2> groundPointsPixeled = {
	Vec2(-5.0f * PIXELS_PER_METER, -0.5f * PIXELS_PER_METER),
	Vec2(5.0f * PIXELS_PER_METER, -0.5f * PIXELS_PER_METER),
	Vec2(5.0f * PIXELS_PER_METER,  0.5f * PIXELS_PER_METER),
	Vec2(-5.0f * PIXELS_PER_METER,  0.5f * PIXELS_PER_METER)
	};
	ground->cTransform = std::make_shared<CTransform>(Vec2(5, 5), 0);
	ground->cShape = std::make_shared<CPolygon>(groundPointsPixeled);
	ground->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::STATIC);
	ground->cRigidbody->m_velocity = Vec2(0, 0);
	auto collider2 = std::make_shared<CPolygonCollider>(groundPoints);
	ground->cCollider = collider2;
	ground->cCollisionState = std::make_shared<CCollisionState>();
	ground->cRigidbody->SetMassAndInertia(100.0, collider2->GetMomentOfInertia());

	const std::vector<Vec2> pentagonPoints = {
	Vec2(0.0f,  1.0f),
	Vec2(-0.951f,  0.309f),
	Vec2(-0.588f, -0.809f),
	Vec2(0.588f, -0.809f),
	Vec2(0.951f,  0.309f)
	};

	const std::vector<Vec2> pentagonPointsPixeled = {
		Vec2(0.0f * PIXELS_PER_METER,  1.0f * PIXELS_PER_METER),
		Vec2(-0.951f * PIXELS_PER_METER,  0.309f * PIXELS_PER_METER),
		Vec2(-0.588f * PIXELS_PER_METER, -0.809f * PIXELS_PER_METER),
		Vec2(0.588f * PIXELS_PER_METER, -0.809f * PIXELS_PER_METER),
		Vec2(0.951f * PIXELS_PER_METER,  0.309f * PIXELS_PER_METER)
	};
	auto pentagon = m_entityManager.AddEntity("OBB");
	pentagon->cTransform = std::make_shared<CTransform>(Vec2(5, 1), 3); // position dans le monde
	//pentagon->cShape = std::make_shared<CPolygon>();
	pentagon->cShape = std::make_shared<CPolygon>(pentagonPointsPixeled);
	pentagon->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	pentagon->cRigidbody->m_velocity = Vec2(0, -1);
	auto pentagonCollider = std::make_shared<CPolygonCollider>(pentagonPoints);
	pentagon->cCollider = pentagonCollider;
	pentagon->cCollisionState = std::make_shared<CCollisionState>();
	pentagon->cRigidbody->SetMassAndInertia(1.0f, pentagonCollider->GetMomentOfInertia());

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
			shape->setRotation(sf::radians(e->cTransform->m_angle));
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
				m_game->ChangeScene("SceneFallingCircle");
			}
		}
	}
}

void SceneFalling::OnEnd()
{

}