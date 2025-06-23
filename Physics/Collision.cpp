#include "Collision.h"

CollisionResponse Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider)
{
	CollisionResponse collisionResponse;
	//Faire un Switch à la place du if
	switch (aCollider.m_type)
	{
	case ColliderType::Circle:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
		{
			collisionResponse = CircleVsCircle(aTransform, static_cast<const CCircleCollider&>(aCollider), bTransform, static_cast<const CCircleCollider&>(bCollider));
			return collisionResponse;
		}
		case ColliderType::AABB:
			return CircleVsAABB(aTransform, static_cast<const CCircleCollider&>(aCollider), bTransform, static_cast<const CAABBCollider&>(bCollider));
		case ColliderType::Polygon:
			return collisionResponse;
		default:
			return collisionResponse;
		}
	case ColliderType::AABB:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
			return CircleVsAABB(bTransform, static_cast<const CCircleCollider&>(bCollider), aTransform, static_cast<const CAABBCollider&>(aCollider));
		case ColliderType::AABB:
			return AABBVsAABB(aTransform, static_cast<const CAABBCollider&>(aCollider), bTransform, static_cast<const CAABBCollider&>(bCollider));
			return collisionResponse;
		case ColliderType::Polygon:
			return collisionResponse;
		default:
			return collisionResponse;
		}
	case ColliderType::Polygon:
		switch (bCollider.m_type)
		{
		case ColliderType::Circle:
			return collisionResponse;
		case ColliderType::AABB:
			return collisionResponse;
		case ColliderType::Polygon:
			return collisionResponse;
		default:
			return collisionResponse;
		}
	default:
		return collisionResponse;
	}
	return collisionResponse;
}

CollisionResponse CircleVsCircle(const CTransform& aTransform, const CCircleCollider& aCollider, const CTransform& bTransform, const CCircleCollider& bCollider)
{
	CollisionResponse collisionResponse;
	Vec2 delta = bTransform.m_position - aTransform.m_position;
	float distSq = delta.x * delta.x + delta.y * delta.y;
	float rSum = aCollider.m_radius + bCollider.m_radius;
	if (distSq <= rSum * rSum)
	{
		collisionResponse.isColliding = true;
		collisionResponse.normal = delta.Normalized();
		collisionResponse.penetration = (aCollider.m_radius + bCollider.m_radius) - sqrt(distSq);
		return collisionResponse;
	}
	else
	{
		return collisionResponse;
	}
}

float Clamp(float value, float min, float max)
{
	return std::max(min, std::min(value, max));
}

CollisionResponse CircleVsAABB(const CTransform& circleTransform, const CCircleCollider& circleCollider,
	const CTransform& aabbTransform, const CAABBCollider& aabbCollider)
{
	CollisionResponse collisionResponse;
	Vec2 boxHalf = aabbCollider.m_size * 0.5f;
	Vec2 boxCenter = aabbTransform.m_position;
	Vec2 circleCenter = circleTransform.m_position;

	float closestX = Clamp(circleCenter.x, boxCenter.x - boxHalf.x, boxCenter.x + boxHalf.x);
	float closestY = Clamp(circleCenter.y, boxCenter.y - boxHalf.y, boxCenter.y + boxHalf.y);

	float dx = circleCenter.x - closestX;
	float dy = circleCenter.y - closestY;

	if ((dx * dx + dy * dy) <= (circleCollider.m_radius * circleCollider.m_radius))
	{
		collisionResponse.isColliding = true;
		return collisionResponse;
	}
	else
	{

		return collisionResponse;
	}

}

CollisionResponse AABBVsAABB(const CTransform& aTransform, const CAABBCollider& aCollider, const CTransform& bTransform, const CAABBCollider& bCollider)
{
	CollisionResponse collisionResponse;

	Vec2 aMin = aTransform.m_position - aCollider.m_size * 0.5f;
	Vec2 aMax = aTransform.m_position + aCollider.m_size * 0.5f;
	Vec2 bMin = bTransform.m_position - bCollider.m_size * 0.5f;
	Vec2 bMax = bTransform.m_position + bCollider.m_size * 0.5f;

	if ((aMin.x <= bMax.x && aMax.x >= bMin.x &&
		aMin.y <= bMax.y && aMax.y >= bMin.y))
	{
		collisionResponse.isColliding = true;
		return collisionResponse;
	}
	else
	{
		return collisionResponse;
	}
}

void CollisionResponseCircleVsCircle(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, CollisionResponse& collisionResponse)
{
	Vec2 normal = (bEntity->cTransform->m_position - aEntity->cTransform->m_position).Normalized();
	Vec2 relativeVelocity = bEntity->cRigidbody->m_velocity - aEntity->cRigidbody->m_velocity;
	float velAlongNormal = Vec2::Dot(relativeVelocity, normal);

	float restitution = 1.0f;

	float j = 0;
	if (aEntity->cRigidbody->m_mass != 0 && bEntity->cRigidbody->m_mass != 0)
	{

		j = -(1 + restitution) * velAlongNormal;
		j /= (1 / aEntity->cRigidbody->m_mass + 1 / bEntity->cRigidbody->m_mass);

		Vec2 impulse = normal * j;

		aEntity->cRigidbody->m_velocity -= impulse / aEntity->cRigidbody->m_mass;
		bEntity->cRigidbody->m_velocity += impulse / bEntity->cRigidbody->m_mass;
	}

};