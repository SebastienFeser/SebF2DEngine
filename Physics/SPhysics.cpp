#include "SPhysics.h"
#include "PhysicsConstants.h"
#include "Collision.h"
#include "../Config/GameConfig.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SPhysics::SPhysics() : m_quadTree(0, Vec2(GameConfig::SCREEN_WIDTH/2, GameConfig::SCREEN_HEIGHT/2), Vec2(GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT)) {};

void SPhysics::Update(EntityManager& entityManager, float dt)
{
	for (auto& e : entityManager.GetEntities())
	{
		if (e->cCollisionState)
		{
			e->cCollisionState->isColliding = false;
		}
		if (e->cTransform && e->cRigidbody)
		{
			auto& rb = e->cRigidbody;

			if (rb->m_bodyType == CRigidbody::BodyType::DYNAMIC)
			{
				//rb->m_velocity += m_gravity * dt;

				rb->AddForce(m_gravity * rb->m_mass); // TODO: Try to optimize this
				rb->Integrate(dt);
				rb->GenerateAndApplyDragForce(dt);


				/*if (rb->m_mass > 0)
				{
					rb->Integrate(dt);
					//Vec2 acceleration = rb->m_force / rb->m_mass;
					//rb->m_velocity += acceleration * dt;

					
				}*/

				//rb->m_force = Vec2(0.f, 0.f);
			}

			if (rb->m_bodyType != CRigidbody::BodyType::STATIC)
			{
				rb->m_angularVelocity *= std::pow(1.0f - rb->m_angularDrag, dt);
				e->cTransform->m_position += rb->m_velocity * dt;
				e->cTransform->m_angle += (rb->m_angularVelocity * 180.0f / M_PI) * dt;
			}
		}
	}

	const auto& entities = entityManager.GetEntities();

	if (m_useQuadTree)
	{
		m_quadTree.Clear();

		for (auto& e : entities)
		{
			if (e->cTransform && e->cCollider && e->cCollider->m_type == ColliderType::AABB)
			{
				m_quadTree.Insert(e);
			}
		}

		for (auto& e : entities)
		{
			if (!e->cTransform || !e->cCollider) continue;

			std::vector<std::shared_ptr<Entity>> candidates;
			m_quadTree.Retrieve(candidates, e);

			for (const auto& other : candidates)
			{
				if (e == other) continue;
				if (!other->cTransform || !other->cCollider) continue;

				bool isColliding = Collision(e, other);

				if (isColliding)
				{
					if (e->cCollisionState) e->cCollisionState->isColliding = true;
					if (other->cCollisionState) other->cCollisionState->isColliding = true;
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < entities.size(); ++i)
		{
			for (size_t j = i + 1; j < entities.size(); ++j)
			{
				auto& a = entities[i];
				auto& b = entities[j];

				if (!a->cTransform || !a->cCollider) continue;
				if (!b->cTransform || !b->cCollider) continue;

				bool isColliding = Collision(a, b);

				if (isColliding)
				{
					if (a->cCollisionState)
					{
						a->cCollisionState->isColliding = true;
					}
					if (b->cCollisionState)
					{
						b->cCollisionState->isColliding = true;
					}
				}
			}
		}
	}
	
}