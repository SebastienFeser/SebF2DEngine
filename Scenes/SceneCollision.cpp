#include "SceneCollision.h"
#include "../Physics/PhysicsConstants.h"

void SceneCollision::Init()
{
	//basketballTexture.loadFromFile("../Assets/Sprites/BasketBall.png");
	
	//basketballSprite->setTexture(basketballTexture);

	auto circle1 = m_entityManager.AddEntity("Circle");
	circle1->cTransform = std::make_shared<CTransform>(Vec2(1, 1));
	circle1->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle1->cShape->SetTexture(&basketballTexture);
	circle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle1->cRigidbody->m_velocity = Vec2(1.5f, 0);
	circle1->cRigidbody->m_bounce = 0.5f;
	circle1->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle1->cCollisionState = std::make_shared<CCollisionState>();
	circle1->cRigidbody->SetMassAndInertia(1.0f, circle1->cCollider->GetMomentOfInertia());

	auto circle2 = m_entityManager.AddEntity("Circle");
	circle2->cTransform = std::make_shared<CTransform>(Vec2(5, 1.1));
	circle2->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle2->cShape->SetTexture(&basketballTexture);
	circle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle2->cRigidbody->m_velocity = Vec2(-1.5f, 0);
	//circle2->cRigidbody->m_mass = 2.0f;
	circle2->cRigidbody->m_bounce = 0.5f;
	circle2->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle2->cCollisionState = std::make_shared<CCollisionState>();
	circle2->cRigidbody->SetMassAndInertia(2.0f, circle2->cCollider->GetMomentOfInertia());

	auto rectangle1 = m_entityManager.AddEntity("Rectangle");
	rectangle1->cTransform = std::make_shared<CTransform>(Vec2(1, 3), 30);
	const std::vector<Vec2> rectanglePoints =
	{
		Vec2(0.0f, 0.0f), // bas gauche
		Vec2(1.0f, 0.0f), // bas droit
		Vec2(1.0f, 1.0f), // haut droit
		Vec2(0.0f, 1.0f)  // haut gauche
	};

	// Conversion en pixels
	const std::vector<Vec2> rectanglePointsPixeled =
	{
		Vec2(0.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(1.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(1.0f * PIXELS_PER_METER, 1.0f * PIXELS_PER_METER),
		Vec2(0.0f * PIXELS_PER_METER, 1.0f * PIXELS_PER_METER)
	};

	rectangle1->cShape = std::make_shared<CPolygon>(rectanglePointsPixeled);
	rectangle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle1->cRigidbody->m_velocity = Vec2(0.5f, 0);
	rectangle1->cCollider = std::make_shared<CPolygonCollider>(rectanglePoints);
	rectangle1->cCollisionState = std::make_shared<CCollisionState>();
	rectangle1->cRigidbody->SetMassAndInertia(1.0, rectangle1->cCollider->GetMomentOfInertia());
	

	auto circle3 = m_entityManager.AddEntity("Circle");
	circle3->cTransform = std::make_shared<CTransform>(Vec2(5, 3));
	circle3->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle3->cShape->SetTexture(&basketballTexture);
	circle3->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle3->cRigidbody->m_velocity = Vec2(-0.5f, 0);
	circle3->cRigidbody->m_bounce = 0.5f;
	circle3->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle3->cCollisionState = std::make_shared<CCollisionState>();
	circle3->cRigidbody->SetMassAndInertia(1.0f, circle3->cCollider->GetMomentOfInertia());


	/*auto rectangle1 = m_entityManager.AddEntity("OBB");
	rectangle1->cTransform = std::make_shared<CTransform>(Vec2(1, 3), 30);
	rectangle1->cShape = std::make_shared<COBB>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER), 30);
	rectangle1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle1->cRigidbody->m_velocity = Vec2(1.5f, 0);
	auto collider1 = std::make_shared<COBBCollider>(Vec2(0.5f, 0.5f), 30);
	rectangle1->cCollider = collider1;
	rectangle1->cCollisionState = std::make_shared<CCollisionState>();
	rectangle1->cRigidbody->SetMassAndInertia(1.0, collider1->GetMomentOfInertia());

	auto rectangle2 = m_entityManager.AddEntity("OBB");
	rectangle2->cTransform = std::make_shared<CTransform>(Vec2(5, 3.2), 45);
	rectangle2->cShape = std::make_shared<COBB>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER), 45);
	rectangle2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle2->cRigidbody->m_velocity = Vec2(-1.5f, 0);
	auto collider2 = std::make_shared<COBBCollider>(Vec2(0.5f, 0.5f), 45);
	rectangle2->cCollider = collider2;
	rectangle2->cCollisionState = std::make_shared<CCollisionState>();
	rectangle2->cRigidbody->SetMassAndInertia(1.0, collider2->GetMomentOfInertia());*/

	auto polygon1 = m_entityManager.AddEntity("Polygon");
	polygon1->cTransform = std::make_shared<CTransform>(Vec2(1, 4.8));
	const std::vector<Vec2> polygonPoints = 
	{
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.5f, 1.0f)
	};
	const std::vector<Vec2> polygonPointsPixeled =
	{
		Vec2(0.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(1.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(0.5f * PIXELS_PER_METER, 1.0f * PIXELS_PER_METER)
	};

	polygon1->cShape = std::make_shared<CPolygon>(polygonPointsPixeled);
	polygon1->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	polygon1->cRigidbody->m_velocity = Vec2(1.0f, 0);
	polygon1->cCollider = std::make_shared<CPolygonCollider>(polygonPoints);
	polygon1->cCollisionState = std::make_shared<CCollisionState>();
	polygon1->cRigidbody->SetMassAndInertia(1.0, polygon1->cCollider->GetMomentOfInertia());

	auto polygon2 = m_entityManager.AddEntity("Polygon");
	polygon2->cTransform = std::make_shared<CTransform>(Vec2(5, 5));
	const std::vector<Vec2> polygonPoints2 =
	{
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.5f, 1.0f)
	};
	const std::vector<Vec2> polygonPoints2Pixeled =
	{
		Vec2(0.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(1.0f * PIXELS_PER_METER, 0.0f * PIXELS_PER_METER),
		Vec2(0.5f * PIXELS_PER_METER, 1.0f * PIXELS_PER_METER)
	};
	polygon2->cShape = std::make_shared<CPolygon>(polygonPoints2Pixeled);
	polygon2->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	polygon2->cRigidbody->m_velocity = Vec2(-1.0f, 0);
	polygon2->cCollider = std::make_shared<CPolygonCollider>(polygonPoints2);
	polygon2->cCollisionState = std::make_shared<CCollisionState>();
	polygon2->cRigidbody->SetMassAndInertia(1.0, polygon2->cCollider->GetMomentOfInertia());

	/*auto rectangle3 = m_entityManager.AddEntity("OrientedRectangle");
	rectangle3->cTransform = std::make_shared<CTransform>(Vec2(5, 5));
	rectangle3->cShape = std::make_shared<COrientedRectangle>(Vec2(0.5f * PIXELS_PER_METER, 0.5f * PIXELS_PER_METER));
	rectangle3->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	rectangle3->cRigidbody->m_velocity = Vec2(-1.0f, 0);
	rectangle3->cCollider = std::make_shared<CAABBCollider>(Vec2(0.5f, 0.5f));
	rectangle3->cCollisionState = std::make_shared<CCollisionState>();

	auto circle3 = m_entityManager.AddEntity("Circle");
	circle3->cTransform = std::make_shared<CTransform>(Vec2(1, 5));
	circle3->cShape = std::make_shared<CCircle>(0.25f * PIXELS_PER_METER);
	circle3->cRigidbody = std::make_shared<CRigidbody>(1.0f, CRigidbody::BodyType::DYNAMIC);
	circle3->cRigidbody->m_velocity = Vec2(1.0f, 0);
	circle3->cCollider = std::make_shared<CCircleCollider>(0.25f);
	circle3->cCollisionState = std::make_shared<CCollisionState>();*/

	m_physics.SetGravity(Vec2(0, 0));
	m_physics.m_useQuadTree = false;
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
			/*if (auto circle = dynamic_cast<CCircle*>(e->cShape.get
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
				m_game->ChangeScene("SceneNoQuadTree");
			}
		}
	}
}

void SceneCollision::OnEnd()
{

}