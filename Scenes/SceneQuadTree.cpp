#pragma once

#include "SceneQuadTree.h"
#include "../Physics/PhysicsConstants.h"
#include "../Config/GameConfig.h"
#include <random>

Vec2 SceneQuadTree::RandomPosition()
{
	float minX = 0;
	float maxX = GameConfig::SCREEN_WIDTH;
	float minY = 0;
	float maxY = GameConfig::SCREEN_HEIGHT;
	float x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
	float y = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);

	return Vec2(x, y);
}

Vec2 SceneQuadTree::RandomScale()
{
	float minX = 10;
	float maxX = 20;
	float minY = 10;
	float maxY = 20;
	float x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
	float y = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);

	return Vec2(x, y);
}

Vec2 SceneQuadTree::RandomVelocity()
{
	float minX = -20.0f;
	float maxX = 20.0f;
	float minY = -20.0f;
	float maxY = 20.0f;
	float x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
	float y = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);

	return Vec2(x, y);
}

void SceneQuadTree::Init()
{
	std::srand(std::time(nullptr));
	for (int i = 0; i < m_aabbCount; i++)
	{
		auto rectangle = m_entityManager.AddEntity("OrientedRectangle");
		rectangle->cTransform = std::make_shared<CTransform>(RandomPosition());
		Vec2 randomScale = RandomScale();
		rectangle->cShape = std::make_shared<COrientedRectangle>(randomScale);
		rectangle->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
		rectangle->cRigidbody->m_velocity = RandomVelocity();
		rectangle->cCollider = std::make_shared<CAABBCollider>(randomScale);
		rectangle->cCollisionState = std::make_shared<CCollisionState>();
	}
	m_physics.m_useQuadTree = true;
	/*auto circle1 = m_entityManager.AddEntity("Circle");
	circle1->cTransform = std::make_shared<CTransform>(Vec2(1, 1));
	circle1->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	circle1->cRigidbody->m_velocity = Vec2(1.5f, 0);
	circle1->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle1->cCollisionState = std::make_shared<CCollisionState>();

	auto circle2 = m_entityManager.AddEntity("Circle");
	circle2->cTransform = std::make_shared<CTransform>(Vec2(5, 1));
	circle2->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	circle2->cRigidbody->m_velocity = Vec2(-1.5f, 0);
	circle2->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle2->cCollisionState = std::make_shared<CCollisionState>();

	auto rectangle1 = m_entityManager.AddEntity("OrientedRectangle");
	rectangle1->cTransform = std::make_shared<CTransform>(Vec2(1, 3));
	rectangle1->cShape = std::make_shared<COrientedRectangle>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER));
	rectangle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	rectangle1->cRigidbody->m_velocity = Vec2(1.5f, 0);
	rectangle1->cCollider = std::make_shared<CAABBCollider>(Vec2(0.5f, 0.5f));
	rectangle1->cCollisionState = std::make_shared<CCollisionState>();

	auto rectangle2 = m_entityManager.AddEntity("OrientedRectangle");
	rectangle2->cTransform = std::make_shared<CTransform>(Vec2(5, 3.2));
	rectangle2->cShape = std::make_shared<COrientedRectangle>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER));
	rectangle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	rectangle2->cRigidbody->m_velocity = Vec2(-1.5f, 0);
	rectangle2->cCollider = std::make_shared<CAABBCollider>(Vec2(0.5f, 0.5f));
	rectangle2->cCollisionState = std::make_shared<CCollisionState>();

	auto rectangle3 = m_entityManager.AddEntity("OrientedRectangle");
	rectangle3->cTransform = std::make_shared<CTransform>(Vec2(1, 5));
	rectangle3->cShape = std::make_shared<COrientedRectangle>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER));
	rectangle3->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	rectangle3->cRigidbody->m_velocity = Vec2(1.0f, 0);
	rectangle3->cCollider = std::make_shared<CAABBCollider>(Vec2(0.5f, 0.5f));
	rectangle3->cCollisionState = std::make_shared<CCollisionState>();

	auto circle3 = m_entityManager.AddEntity("Circle");
	circle3->cTransform = std::make_shared<CTransform>(Vec2(5, 5));
	circle3->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle3->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::KINEMATIC);
	circle3->cRigidbody->m_velocity = Vec2(-1.0f, 0);
	circle3->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle3->cCollisionState = std::make_shared<CCollisionState>();*/
}

void SceneQuadTree::Update(float dt)
{
	m_entityManager.Update();
	m_physics.Update(m_entityManager, dt);
	for (auto& e : m_entityManager.GetEntities())
	{
		if (e->cTransform && e->cRigidbody)
		{
			if (e->cTransform->m_position.x > GameConfig::SCREEN_WIDTH)
			{
				e->cTransform->m_position.x = 0;
			}
			else if (e->cTransform->m_position.x < 0)
			{
				e->cTransform->m_position.x = GameConfig::SCREEN_WIDTH;
			}
			if (e->cTransform->m_position.y > GameConfig::SCREEN_HEIGHT)
			{
				e->cTransform->m_position.y = 0;
			}
			else if (e->cTransform->m_position.y < 0)
			{
				e->cTransform->m_position.y = GameConfig::SCREEN_HEIGHT;
			}
		}
	}
}

void SceneQuadTree::Render(sf::RenderWindow& m_window)
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
			shape->setPosition(sf::Vector2f(e->cTransform->m_position.x, e->cTransform->m_position.y));
			/*if (auto circle = dynamic_cast<CCircle*>(e->cShape.get()))
			{
				float radius = circle->GetRadius();
				shape->setOrigin(sf::Vector2f(radius, radius));
				shape->setScale(sf::Vector2f(radius * PIXELS_PER_METER, radius * PIXELS_PER_METER));
			}*/
			m_window.draw(*shape);
		}
	}

	//m_window.display();
}

void SceneQuadTree::ProcessInput(sf::RenderWindow& m_window)
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

void SceneQuadTree::OnEnd()
{

}