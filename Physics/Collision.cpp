#include "Collision.h"

bool Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider)
{
	//Faire un Switch à la place du if
	switch (aCollider.m_type)
	{
	case ColliderType::Circle:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
			return CircleVsCircle(aTransform, static_cast<const CCircleCollider&>(aCollider), bTransform, static_cast<const CCircleCollider&>(bCollider));
		case ColliderType::AABB:
			return CircleVsAABB(aTransform, static_cast<const CCircleCollider&>(aCollider), bTransform, static_cast<const CAABBCollider&>(bCollider));
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	case ColliderType::AABB:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
			return CircleVsAABB(bTransform, static_cast<const CCircleCollider&>(bCollider), aTransform, static_cast<const CAABBCollider&>(aCollider));
		case ColliderType::AABB:
			return AABBVsAABB(aTransform, static_cast<const CAABBCollider&>(aCollider), bTransform, static_cast<const CAABBCollider&>(bCollider));
			return false;
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	case ColliderType::Polygon:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
			return false;
		case ColliderType::AABB:
			return false;
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	default:
		return false;
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

float Clamp(float value, float min, float max)
{
	return std::max(min, std::min(value, max));
}

bool CircleVsAABB(const CTransform& circleTransform, const CCircleCollider& circleCollider,
	const CTransform& aabbTransform, const CAABBCollider& aabbCollider)
{
	Vec2 boxHalf = aabbCollider.m_size * 0.5f;
	Vec2 boxCenter = aabbTransform.m_position;
	Vec2 circleCenter = circleTransform.m_position;

	float closestX = Clamp(circleCenter.x, boxCenter.x - boxHalf.x, boxCenter.x + boxHalf.x);
	float closestY = Clamp(circleCenter.y, boxCenter.y - boxHalf.y, boxCenter.y + boxHalf.y);

	float dx = circleCenter.x - closestX;
	float dy = circleCenter.y - closestY;

	return (dx * dx + dy * dy) <= (circleCollider.m_radius * circleCollider.m_radius);
}

bool AABBVsAABB(const CTransform& aTransform, const CAABBCollider& aCollider, const CTransform& bTransform, const CAABBCollider& bCollider)
{
	Vec2 aMin = aTransform.m_position - aCollider.m_size * 0.5f;
	Vec2 aMax = aTransform.m_position + aCollider.m_size * 0.5f;
	Vec2 bMin = bTransform.m_position - bCollider.m_size * 0.5f;
	Vec2 bMax = bTransform.m_position + bCollider.m_size * 0.5f;

	return (aMin.x <= bMax.x && aMax.x >= bMin.x &&
		aMin.y <= bMax.y && aMax.y >= bMin.y);
}