#pragma once

#include "../Maths/Vec2.h"
#include <vector>
#include <memory>

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
	float m_angle;
	CPolygonCollider(const std::vector<Vec2>& points) : CCollider(ColliderType::Polygon), m_points(points) {}

	float GetMomentOfInertia() const override
	{
		float numerator = 0.0f;
		float area = 0.0f;

		const std::vector<Vec2>& verts = m_points;
		const int count = static_cast<int>(verts.size());

		for (int i = 0; i < count; ++i)
		{
			const Vec2& p0 = verts[i];
			const Vec2& p1 = verts[(i + 1) % count];

			float cross = Vec2::Cross(p0, p1);
			float term = Vec2::Dot(p0, p0) + Vec2::Dot(p0,p1) + Vec2::Dot(p1,p1);

			numerator += cross * term;
			area += cross;
		}

		area *= 0.5f;
		if (std::abs(area) < 1e-6f) return 0.0f; 

		float inertia = (1.0f / 6.0f) * numerator;

		return std::abs(inertia);
	}

	std::vector<Vec2> GetWorldPoints(std::shared_ptr<CTransform>& transform) const
	{
		std::vector<Vec2> worldPoints;
		worldPoints.reserve(m_points.size());
		for (const Vec2& p : m_points)
		{
			Vec2 rotated = Vec2::Rotate(p, transform->m_angle);     // Applique la rotation
			Vec2 translated = rotated + transform->m_position;   // Applique la position
			worldPoints.push_back(translated);
		}
		return worldPoints;
	}

	Vec2 EdgeAt(int index, std::shared_ptr<CTransform>& transform) const
	{
		const std::vector<Vec2> worldPoints = GetWorldPoints(transform);
		int currVertex = index;
		int nextVertex = (index + 1) % worldPoints.size();

		return worldPoints[nextVertex] - worldPoints[currVertex];
	}

};