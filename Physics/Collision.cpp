#include "Collision.h"

bool Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider)
{
	//Faire un Switch à la place du if
	if (aCollider.m_type == CCollider::Type::Circle)
	{
		if (bCollider.m_type == CCollider::Type::Circle)
		{
			return CircleVsCircle(aTransform, aCollider, bTransform, bCollider);
		}
	}
	return false;
}

bool CircleVsCircle(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider)
{
	Vec2 delta = bTransform.m_position - aTransform.m_position;
	float distSq = delta.x * delta.x + delta.y * delta.y;
	float rSum = aCollider.m_radius + bCollider.m_radius;
	return distSq <= rSum * rSum;
}