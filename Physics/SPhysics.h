#pragma once

#include "../ECS/EntityManager.h"
#include "../Maths/Vec2.h"

class SPhysics 
{
	Vec2 m_gravity = Vec2(0.f, 9.81f);
public:
	void Update(EntityManager& entityManager, float dt);
	void SetGravity(const Vec2& gravity) { m_gravity = gravity; }
	Vec2 GetGravity() const { return m_gravity; }
};