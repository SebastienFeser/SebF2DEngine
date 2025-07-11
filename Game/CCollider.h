#pragma once

#include "../Maths/Vec2.h"
#include <vector>

enum class ColliderType { Circle, AABB, OBB, Polygon };

class CCollider
{
public:
	
	ColliderType m_type = ColliderType::Circle;

	//float m_radius = 0.f;
	//Vec2 m_size = { 0, 0 };
	//std::vector<Vec2> m_points;

	bool m_isTrigger = false;

	virtual ~CCollider() = default;
	virtual float GetMomentOfInertia() const = 0;
protected:
	CCollider(ColliderType type) : m_type(type) {}
};

class CCircleCollider : public CCollider
{
public:
	float m_radius;
	CCircleCollider(float radius) : CCollider(ColliderType::Circle), m_radius(radius) {}
	float GetMomentOfInertia() const override
	{
		return 0.5 * m_radius * m_radius;
	};
};

class CAABBCollider : public CCollider
{
public:
	Vec2 m_size;
	CAABBCollider(const Vec2& size) : CCollider(ColliderType::AABB), m_size(size) {}
	float GetMomentOfInertia() const override 
	{
		return 0.083333 * (m_size.x * m_size.x + m_size.y * m_size.y);
	};
};

class COBBCollider : public CCollider
{
public:
	Vec2 m_size;
	float m_angle;
	COBBCollider(const Vec2& size, float angle) : CCollider(ColliderType::OBB), m_size(size), m_angle(angle) {}
	float GetMomentOfInertia() const override
	{
		return 0.083333 * (m_size.x * m_size.x + m_size.y * m_size.y);
	};
};

class CPolygonCollider : public CCollider
{
public:
	std::vector<Vec2> m_points;
	CPolygonCollider(const std::vector<Vec2>& points) : CCollider(ColliderType::Polygon), m_points(points) {}
	float GetMomentOfInertia() const override
	{
		//TODO
		return 0.0;
	};
};