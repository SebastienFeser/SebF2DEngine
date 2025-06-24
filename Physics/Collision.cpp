#include "Collision.h"
#include <algorithm>

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
		collisionResponse.normal = delta / sqrt(distSq);
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
	if (collisionResponse.penetration <= 0) return;

	auto rbA = aEntity->cRigidbody;
	auto rbB = bEntity->cRigidbody;

	auto typeA = rbA ? rbA->m_bodyType : CRigidbody::BodyType::STATIC;
	auto typeB = rbB ? rbB->m_bodyType : CRigidbody::BodyType::STATIC;

	if ((typeA != CRigidbody::BodyType::DYNAMIC) && (typeB != CRigidbody::BodyType::DYNAMIC) || (typeA == CRigidbody::BodyType::KINEMATIC) || (typeB == CRigidbody::BodyType::KINEMATIC)) return;


	//Vec2& posA = aEntity->cTransform->m_position;
	//Vec2& posB = bEntity->cTransform->m_position;
	Vec2 dummyVel = Vec2();
	Vec2& velA = rbA ? rbA->m_velocity : dummyVel;
	Vec2& velB = rbB ? rbB->m_velocity : dummyVel;
	float massA = (typeA == CRigidbody::BodyType::DYNAMIC) ? rbA->m_mass : std::numeric_limits<float>::infinity();
	float massB = (typeB == CRigidbody::BodyType::DYNAMIC) ? rbB->m_mass : std::numeric_limits<float>::infinity();
	float bounceA = rbA ? rbA->m_bounce : 1.0f;
	float bounceB = rbB ? rbB->m_bounce : 1.0f;

	Vec2 relVelocity = velB - velA;
	float velAlongNormal = Vec2::Dot(relVelocity, collisionResponse.normal);

	if (velAlongNormal > 0) return;

	//float e = std::min(bounceA, bounceB);
	//float j = -(1.0f + e) * velAlongNormal;
	float j = -2.0f * velAlongNormal;
	j /= 1.0f / massA + 1.0f / massB;

	Vec2 impulse = collisionResponse.normal * j;
	if (typeA == CRigidbody::BodyType::DYNAMIC && rbA)
	{
		velA -= impulse / massA;
		velA *= bounceA;
	}

	if (typeB == CRigidbody::BodyType::DYNAMIC && rbB)
	{
		velB += impulse / massB;
		velB *= bounceB;
	}
		
	Vec2& posA = aEntity->cTransform->m_position;
	Vec2& posB = bEntity->cTransform->m_position;

	float totalMass = (1.0f / massA) + (1.0f / massB);
	if (totalMass == 0) return;

	Vec2 correction = collisionResponse.normal * (collisionResponse.penetration / totalMass);

	if (typeA == CRigidbody::BodyType::DYNAMIC)
		posA -= correction * (1.0f / massA);

	if (typeB == CRigidbody::BodyType::DYNAMIC)
		posB += correction * (1.0f / massB);
}