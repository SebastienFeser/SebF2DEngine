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
		case ColliderType::OBB:
			return false;
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
		case ColliderType::OBB:
			return false;
		case ColliderType::Polygon:
			return false;
		default:
			return false;
		}
	case ColliderType::OBB:
		switch (bCollider->m_type)
		{
		case ColliderType::Circle:
			return false;
		case ColliderType::AABB:
			return false;
		case ColliderType::OBB:
			return OBBvsOBB(aEntity, bEntity);
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
		case ColliderType::OBB:
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

void ProjectOBB(const Vec2& center, const Vec2& halfSize, const Vec2& axis, float angle, float& min, float& max)
{
	Vec2 corners[4];
	Vec2 xAxis = Vec2(std::cos(angle), std::sin(angle));
	Vec2 yAxis = Vec2(-std::sin(angle), std::cos(angle));

	corners[0] = center + xAxis * halfSize.x + yAxis * halfSize.y;
	corners[1] = center + xAxis * halfSize.x - yAxis * halfSize.y;
	corners[2] = center - xAxis * halfSize.x + yAxis * halfSize.y;
	corners[3] = center - xAxis * halfSize.x - yAxis * halfSize.y;

	min = max = Vec2::Dot(corners[0], axis);
	for (int i = 1; i < 4; ++i)
	{
		float projection = Vec2::Dot(corners[i], axis);
		if (projection < min) min = projection;
		if (projection > max) max = projection;
	}
}

bool OBBvsOBB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity)
{
	auto aCollider = std::static_pointer_cast<COBBCollider>(aEntity->cCollider);
	auto bCollider = std::static_pointer_cast<COBBCollider>(bEntity->cCollider);

	auto aPos = aEntity->cTransform->m_position;
	auto bPos = bEntity->cTransform->m_position;

	Vec2 axes[4] = {
		Vec2(std::cos(aCollider->m_angle), std::sin(aCollider->m_angle)),                  // axis A1
		Vec2(-std::sin(aCollider->m_angle), std::cos(aCollider->m_angle)),                 // axis A2
		Vec2(std::cos(bCollider->m_angle), std::sin(bCollider->m_angle)),                  // axis B1
		Vec2(-std::sin(bCollider->m_angle), std::cos(bCollider->m_angle))                  // axis B2
	};

	float minPenetration = INFINITY;
	Vec2 smallestAxis;

	for (int i = 0; i < 4; ++i)
	{
		float minA, maxA;
		float minB, maxB;

		ProjectOBB(aPos, aCollider->m_size/2, axes[i], aCollider->m_angle, minA, maxA);
		ProjectOBB(bPos, bCollider->m_size/2, axes[i], bCollider->m_angle, minB, maxB);

		if (maxA < minB || maxB < minA)
		{
			return false;
		}

		float overlap = std::min(maxA, maxB) - std::max(minA, minB);
		if (overlap < minPenetration)
		{
			minPenetration = overlap;
			smallestAxis = axes[i];
		}
	}

	Vec2 direction = bPos - aPos;
	if (Vec2::Dot(direction, smallestAxis) < 0)
	{
		smallestAxis = -smallestAxis;
	}

	CollisionResponseOBBVsOBB(aEntity, bEntity, smallestAxis, minPenetration);

	return true;
}

void CollisionResponseOBBVsOBB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, const Vec2& collisionNormal, float penetrationDepth)
{
	auto& va = aEntity->cRigidbody->m_velocity;
	auto& vb = bEntity->cRigidbody->m_velocity;

	auto& wa = aEntity->cRigidbody->m_angularVelocity;
	auto& wb = bEntity->cRigidbody->m_angularVelocity;

	auto ma = aEntity->cRigidbody->m_inverseMass;
	auto mb = bEntity->cRigidbody->m_inverseMass;

	auto ia = aEntity->cRigidbody->m_inverseInertia;
	auto ib = bEntity->cRigidbody->m_inverseInertia;

	Vec2 contactPoint = (aEntity->cTransform->m_position + bEntity->cTransform->m_position) * 0.5f;

	Vec2 ra = contactPoint - aEntity->cTransform->m_position;
	Vec2 rb = contactPoint - bEntity->cTransform->m_position;

	Vec2 va_contact = va + Vec2(-ra.y, ra.x) * wa;
	Vec2 vb_contact = vb + Vec2(-rb.y, rb.x) * wb;

	Vec2 relativeVelocity = vb_contact - va_contact;

	float contactVel = Vec2::Dot(relativeVelocity, collisionNormal);

	if (contactVel > 0)
		return;

	float raCrossN = Vec2::Cross(ra, collisionNormal);
	float rbCrossN = Vec2::Cross(rb, collisionNormal);
	float invMassSum = ma + mb + (raCrossN * raCrossN) * ia + (rbCrossN * rbCrossN) * ib;

	//float e = std::min(a->cRigidBody->restitution, b->cRigidBody->restitution);
	float j = -2 * contactVel / invMassSum;

	Vec2 impulse = collisionNormal * j;

	va -= impulse * ma;
	vb += impulse * mb;

	wa -= raCrossN * j * ia;
	wb += rbCrossN * j * ib;

	const float percent = 0.8f; 
	const float slop = 0.01f;   
	Vec2 correction = collisionNormal * std::max(penetrationDepth - slop, 0.0f) / (ma + mb) * percent;
	aEntity->cTransform->m_position -= correction * ma;
	bEntity->cTransform->m_position += correction * mb;
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