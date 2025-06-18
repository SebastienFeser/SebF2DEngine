#include "SPhysics.h"
#include "PhysicsConstants.h"

void SPhysics::Update(EntityManager& entityManager, float dt)
{
	for (auto& e : entityManager.GetEntities())
	{
		if (e->cTransform && e->cRigidbody)
		{
			auto& rb = e->cRigidbody;

			if (rb->m_bodyType == CRigidbody::BodyType::DYNAMIC)
			{
				//rb->m_force += m_gravity * rb->m_mass;

				//Vec2 acceleration = rb->m_force / rb->m_mass;
				rb->m_velocity += m_gravity * dt;

				if (rb->m_mass > 0)
				{
					Vec2 acceleration = rb->m_force / rb->m_mass;
					rb->m_velocity += acceleration * dt;
				}

				rb->m_force = Vec2(0.f, 0.f);
			}

			if (rb->m_bodyType != CRigidbody::BodyType::STATIC)
			{
				e->cTransform->m_position += rb->m_velocity * dt;
				e->cTransform->m_angle += rb->m_angularVelocity * dt;
			}
		}
		/*if (e->cTransform && e->cRigidbody && !(e->cRigidbody->m_bodyType == CRigidbody::BodyType::STATIC))
		{
			e->cTransform->m_position += e->cRigidbody->m_velocity * dt;
			e->cTransform->m_angle += e->cRigidbody->m_angularVelocity * dt;
		}*/
	}
}