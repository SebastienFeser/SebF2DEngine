#include "Collision.h"
#include <algorithm>

bool Collision(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity)
{
	//CollisionResponse collisionResponse;
	//Faire un Switch à la place du if

	auto aCollider = aEntity->cCollider;
	auto bCollider = bEntity->cCollider;

	switch (aCollider->m_type)
	{
	case ColliderType::Circle:
		switch (bCollider->m_type)
		{
		case ColliderType::Circle:
		{
			return CircleVsCircle(aEntity, bEntity);;
		}
		case ColliderType::AABB:
			return CircleVsAABB(aEntity, bEntity);
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	case ColliderType::AABB:
		switch (bCollider->m_type)
		{
		case ColliderType::Circle:
			return CircleVsAABB(bEntity, aEntity);
		case ColliderType::AABB:
			return AABBVsAABB(aEntity, bEntity);
			return false;
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	case ColliderType::Polygon:
		switch (bCollider->m_type)
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

bool CircleVsCircle(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity)
{
	auto aCollider = static_pointer_cast<CCircleCollider>(aEntity->cCollider);
	auto bCollider = static_pointer_cast<CCircleCollider>(bEntity->cCollider);
	auto aTransform = aEntity->cTransform;
	auto bTransform = bEntity->cTransform;

	CollisionResponse collisionResponse;
	Vec2 delta = bTransform->m_position - aTransform->m_position;
	float distSq = delta.x * delta.x + delta.y * delta.y;
	float rSum = aCollider->m_radius + bCollider->m_radius;
	if (distSq <= rSum * rSum)
	{
		collisionResponse.isColliding = true;
		collisionResponse.normal = delta / sqrt(distSq);
		collisionResponse.penetration = (aCollider->m_radius + bCollider->m_radius) - sqrt(distSq);
		CollisionResponseCircleVsCircle(aEntity, bEntity, collisionResponse);
		return collisionResponse.isColliding;
	}
	else
	{
		return collisionResponse.isColliding;
	}
}

float Clamp(float value, float min, float max)
{
	return std::max(min, std::min(value, max));
}

bool CircleVsAABB(std::shared_ptr<Entity> circleEntity, std::shared_ptr<Entity> AABBEntity)
{
	auto circleCollider = static_pointer_cast<CCircleCollider>(circleEntity->cCollider);
	auto aabbCollider = static_pointer_cast<CAABBCollider>(AABBEntity->cCollider);
	auto circleTransform = circleEntity->cTransform;
	auto aabbTransform = AABBEntity->cTransform;

	CollisionResponse collisionResponse;
	//collisionResponse.isColliding = false;
	//collisionResponse.penetration = 0.0f;

	Vec2 boxHalf = aabbCollider->m_size * 0.5f;
	Vec2 boxCenter = aabbTransform->m_position;
	Vec2 circleCenter = circleTransform->m_position;

	float closestX = Clamp(circleCenter.x, boxCenter.x - boxHalf.x, boxCenter.x + boxHalf.x);
	float closestY = Clamp(circleCenter.y, boxCenter.y - boxHalf.y, boxCenter.y + boxHalf.y);

	float dx = circleCenter.x - closestX;
	float dy = circleCenter.y - closestY;

	float distSq = dx * dx + dy * dy;
	float radiusSq = circleCollider->m_radius * circleCollider->m_radius;

	if (distSq > radiusSq)
	{
		return collisionResponse.isColliding;
	}

	float dist = std::sqrt(distSq);
	collisionResponse.isColliding = true;

	if (dist == 0.0f)
	{
		collisionResponse.penetration = circleCollider->m_radius;
		collisionResponse.normal = Vec2(1.0f, 0.0f);
	}
	else
	{
		collisionResponse.penetration = circleCollider->m_radius - dist;
		collisionResponse.normal = -Vec2(dx / dist, dy / dist);
	}

	CollisionResponseCircleVsAABB(circleEntity, AABBEntity, collisionResponse);

	return collisionResponse.isColliding;

}

bool AABBVsAABB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity)
{
	auto aCollider = static_pointer_cast<CAABBCollider>(aEntity->cCollider);
	auto bCollider = static_pointer_cast<CAABBCollider>(bEntity->cCollider);
	auto aTransform = aEntity->cTransform;
	auto bTransform = bEntity->cTransform;

	CollisionResponse collisionResponse;

	Vec2 aMin = aTransform->m_position - aCollider->m_size * 0.5f;
	Vec2 aMax = aTransform->m_position + aCollider->m_size * 0.5f;
	Vec2 bMin = bTransform->m_position - bCollider->m_size * 0.5f;
	Vec2 bMax = bTransform->m_position + bCollider->m_size * 0.5f;

	if (aMax.x < bMin.x || aMin.x > bMax.x ||
		aMax.y < bMin.y || aMin.y > bMax.y)
	{
		return collisionResponse.isColliding;
	}

	collisionResponse.isColliding = true;

	if (aEntity->cRigidbody->m_bodyType == CRigidbody::BodyType::KINEMATIC || bEntity->cRigidbody->m_bodyType == CRigidbody::BodyType::KINEMATIC)
	{
		return collisionResponse.isColliding;
	}

	float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
	float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

	if (overlapX < overlapY)
	{
		collisionResponse.penetration = overlapX;
		collisionResponse.normal = (aTransform->m_position.x < bTransform->m_position.x) ? Vec2(1.0f, 0.0f) : Vec2(-1.0f, 0.0f);
	}
	else
	{
		collisionResponse.penetration = overlapY;
		collisionResponse.normal = (aTransform->m_position.y < bTransform->m_position.y) ? Vec2(0.0f, 1.0f) : Vec2(0.0f, -1.0f);
	}

	CollisionResponseAABBVsAABB(aEntity, bEntity, collisionResponse);

	return collisionResponse.isColliding;
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
	
	Vec2 correction;

	if (typeA == CRigidbody::BodyType::STATIC || typeB == CRigidbody::BodyType::STATIC)
	{
		correction = collisionResponse.normal * 2 * (collisionResponse.penetration / totalMass);
	}
	else
	{
		correction = collisionResponse.normal * (collisionResponse.penetration / totalMass);
	}
	

	if (typeA == CRigidbody::BodyType::DYNAMIC)
		posA -= correction * (1.0f / massA);

	if (typeB == CRigidbody::BodyType::DYNAMIC)
		posB += correction * (1.0f / massB);
}

void CollisionResponseCircleVsAABB(std::shared_ptr<Entity> circleEntity, std::shared_ptr<Entity> AABBEntity, CollisionResponse& collisionResponse)
{
	if (collisionResponse.penetration <= 0) return;

	auto rbA = circleEntity->cRigidbody;
	auto rbB = AABBEntity->cRigidbody;

	auto typeA = rbA ? rbA->m_bodyType : CRigidbody::BodyType::STATIC;
	auto typeB = rbB ? rbB->m_bodyType : CRigidbody::BodyType::STATIC;

	if (typeA == CRigidbody::BodyType::KINEMATIC || typeB == CRigidbody::BodyType::KINEMATIC)
		return;

	if (typeA != CRigidbody::BodyType::DYNAMIC && typeB != CRigidbody::BodyType::DYNAMIC)
		return;

	Vec2 dummyVel = Vec2();
	Vec2& velA = (rbA && typeA == CRigidbody::BodyType::DYNAMIC) ? rbA->m_velocity : dummyVel;
	Vec2& velB = (rbB && typeB == CRigidbody::BodyType::DYNAMIC) ? rbB->m_velocity : dummyVel;

	float massA = (typeA == CRigidbody::BodyType::DYNAMIC) ? rbA->m_mass : std::numeric_limits<float>::infinity();
	float massB = (typeB == CRigidbody::BodyType::DYNAMIC) ? rbB->m_mass : std::numeric_limits<float>::infinity();

	float bounceA = rbA ? rbA->m_bounce : 1.0f;
	float bounceB = rbB ? rbB->m_bounce : 1.0f;

	Vec2 relVelocity = velB - velA;
	float velAlongNormal = Vec2::Dot(relVelocity, collisionResponse.normal);

	if (velAlongNormal > 0) return;

	float e = std::min(bounceA, bounceB);
	float j = -(1.0f + e) * velAlongNormal;
	j /= (1.0f / massA + 1.0f / massB);

	Vec2 impulse = collisionResponse.normal * j;

	if (typeA == CRigidbody::BodyType::DYNAMIC)
	{
		velA -= impulse / massA;
		velA *= bounceA;
	}

	if (typeB == CRigidbody::BodyType::DYNAMIC)
	{
		velB += impulse / massB;
		velB *= bounceB;
	}

	Vec2& posA = circleEntity->cTransform->m_position;
	Vec2& posB = AABBEntity->cTransform->m_position;

	float totalMass = (1.0f / massA) + (1.0f / massB);
	if (totalMass == 0) return;

	Vec2 correction = collisionResponse.normal * (collisionResponse.penetration / totalMass);

	if (typeA == CRigidbody::BodyType::DYNAMIC)
		posA -= correction * (1.0f / massA);

	if (typeB == CRigidbody::BodyType::DYNAMIC)
		posB += correction * (1.0f / massB);
}

void CollisionResponseAABBVsAABB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, CollisionResponse& collisionResponse)
{
	if (collisionResponse.penetration <= 0) return;

	auto rbA = aEntity->cRigidbody;
	auto rbB = bEntity->cRigidbody;

	auto typeA = rbA ? rbA->m_bodyType : CRigidbody::BodyType::STATIC;
	auto typeB = rbB ? rbB->m_bodyType : CRigidbody::BodyType::STATIC;

	// Ignore si les deux sont statiques ou si l’un est kinematic
	if ((typeA != CRigidbody::BodyType::DYNAMIC) && (typeB != CRigidbody::BodyType::DYNAMIC)) return;
	if (typeA == CRigidbody::BodyType::KINEMATIC || typeB == CRigidbody::BodyType::KINEMATIC) return;

	Vec2 dummyVel = Vec2();
	Vec2& velA = rbA && typeA == CRigidbody::BodyType::DYNAMIC ? rbA->m_velocity : dummyVel;
	Vec2& velB = rbB && typeB == CRigidbody::BodyType::DYNAMIC ? rbB->m_velocity : dummyVel;

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
	j /= (1.0f / massA + 1.0f / massB);

	Vec2 impulse = collisionResponse.normal * j;

	if (typeA == CRigidbody::BodyType::DYNAMIC)
	{
		velA -= impulse / massA;
		velA *= bounceA;
	}

	if (typeB == CRigidbody::BodyType::DYNAMIC)
	{
		velB += impulse / massB;
		velB *= bounceB;
	}

	Vec2& posA = aEntity->cTransform->m_position;
	Vec2& posB = bEntity->cTransform->m_position;

	float totalMassInverse = (1.0f / massA) + (1.0f / massB);
	if (totalMassInverse == 0) return;

	Vec2 correction = collisionResponse.normal * (collisionResponse.penetration / totalMassInverse);

	if (typeA == CRigidbody::BodyType::DYNAMIC)
		posA -= correction * (1.0f / massA);

	if (typeB == CRigidbody::BodyType::DYNAMIC)
		posB += correction * (1.0f / massB);
}