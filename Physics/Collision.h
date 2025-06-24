#pragma once
#include "../Game/CTransform.h"
#include "../Game/CCollider.h"
#include "../ECS/Entity.h"

struct CollisionResponse
{
	Vec2 normal = Vec2(0,0);
	float penetration = 0.f;
	bool isColliding = false;
};

CollisionResponse Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider);
CollisionResponse CircleVsCircle(const CTransform& aTransform, const CCircleCollider& aCollider, const CTransform& bTransform, const CCircleCollider& bCollider);
CollisionResponse CircleVsAABB(const CTransform& circleTransform, const CCircleCollider& circleCollider, const CTransform& aabbTransform, const CAABBCollider& aabbCollider);
CollisionResponse AABBVsAABB(const CTransform& aTransform, const CAABBCollider&, const CTransform& bTransform, const CAABBCollider& bCollider);

void CollisionResponseCircleVsCircle(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, CollisionResponse& collisionResponse);
void CollisionResponseCircleVsRect(std::shared_ptr<Entity> circleEntity, std::shared_ptr<Entity> rectEntity, CollisionResponse& collisionResponse);
