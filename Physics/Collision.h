#pragma once
#include "../Game/CTransform.h"
#include "../Game/CCollider.h"

bool Collision(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider);
bool CircleVsCircle(const CTransform& aTransform, const CCollider& aCollider, const CTransform& bTransform, const CCollider& bCollider);