#pragma once
#include "../Game/CTransform.h"
#include "../Game/CCollider.h"
#include "../ECS/Entity.h"
#include "Contact.h"

struct CollisionResponse
{
	Vec2 normal = Vec2(0,0);
	float penetration = 0.f;
	bool isColliding = false;
};

bool Collision(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity);
bool CircleVsCircle(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity);
bool CircleVsAABB(std::shared_ptr<Entity> circleEntity, std::shared_ptr<Entity> AABBEntity);
bool AABBVsAABB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity);
bool OBBvsOBB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity);
bool PolygonVsPolygon(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity);

void CollisionResponseCircleVsCircle(Contact& contact);
void CollisionResponseCircleVsAABB(std::shared_ptr<Entity> circleEntity, std::shared_ptr<Entity> AABBEntity, CollisionResponse& collisionResponse);
void CollisionResponseAABBVsAABB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, CollisionResponse& collisionResponse);
void CollisionResponseOBBVsOBB(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, const Vec2& collisionNormal, float penetrationDepth);
void CollisionResponsePolygonVsPolygon(std::shared_ptr<Entity> aEntity, std::shared_ptr<Entity> bEntity, Contact& contact);