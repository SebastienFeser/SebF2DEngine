#pragma once
#include "../Game/CTransform.h"
#include "../Game/CCollider.h"

bool Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider);
bool CircleVsCircle(const CTransform& aTransform, const CCircleCollider& aCollider, const CTransform& bTransform, const CCircleCollider& bCollider);
bool CircleVsAABB(const CTransform& circleTransform, const CCircleCollider& circleCollider, const CTransform& aabbTransform, const CAABBCollider& aabbCollider);
bool AABBVsAABB(const CTransform& aTransform, const CAABBCollider&, const CTransform& bTransform, const CAABBCollider& bCollider);