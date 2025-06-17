#pragma once

#include "../Maths/Vec2.h"
#include <vector>

class CCollider
{
public:
	enum class Type {Circle, AABB, Polygon};
	Type m_type = Type::Circle;

	float m_radius = 0.f;
	Vec2 m_size = { 0, 0 };
	std::vector<Vec2> m_points;

	bool m_isTrigger = false;
	bool m_isStatic = false;

	CCollider(Type type) : m_type(type) {};
};