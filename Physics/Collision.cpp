#include "Collision.h"

bool Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider)
{
	//Faire un Switch à la place du if
	if (aCollider.m_type == ColliderType::Circle)
	{
		if (bCollider.m_type == ColliderType::Circle)
		{
			return CircleVsCircle(aTransform, static_cast<const CCircleCollider&>(aCollider), bTransform, static_cast<const CCircleCollider&>(bCollider));
		}
	}
	return false;
}

bool CircleVsCircle(const CTransform& aTransform, const CCircleCollider& aCollider, const CTransform& bTransform, const CCircleCollider& bCollider)
{
	Vec2 delta = bTransform.m_position - aTransform.m_position;
	float distSq = delta.x * delta.x + delta.y * delta.y;
	float rSum = aCollider.m_radius + bCollider.m_radius;
	return distSq <= rSum * rSum;
}