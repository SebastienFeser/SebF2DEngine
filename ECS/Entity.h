#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "../Game/CTransform.h"
#include "../Game/CRigidbody.h"
#include "../Game/CShape.h"
#include "../Game/CCollider.h"

class Entity {
	const size_t m_id = 0;
	const std::string m_tag = "Default";
	bool m_alive = true;
public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CRigidbody> cRigidbody;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollider> cCollider;
	Entity(const std::string& tag, size_t id);
	void Destroy() { m_alive = false; }
	std::string GetTag() const;
	bool GetAlive() const;
};