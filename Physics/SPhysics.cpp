#include "SPhysics.h"

void SPhysics::Update(EntityManager& entityManager, float dt)
{
	for (auto& e : entityManager.GetEntities())
	{
		if (e->cTransform && e->cRigidbody && !e->cRigidbody->m_isStatic)
		{
			e->cTransform->m_position += e->cRigidbody->m_velocity * dt;
			e->cTransform->m_angle += e->cRigidbody->m_angularVelocity * dt;
		}
	}
}