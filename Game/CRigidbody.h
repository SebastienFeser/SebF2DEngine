#pragma once
#include "../Maths/Vec2.h"

class CRigidbody {
public:
	enum class BodyType
	{
		STATIC,
		DYNAMIC,
		KINEMATIC
	};
	float m_mass = 1.f;
	float m_inverseMass = 0.0f;
	float m_inverseInertia = 0.0f;
	Vec2 m_velocity = { 0.f,0.f };
	Vec2 m_force = { 0.f, 0.f };
	float m_angularVelocity = 0.f;
	float m_angularDrag = 0.98f;
	float m_torque = 0.0f;
	float m_friction = 0.1f;
	float m_bounce = 0.3f;
	BodyType m_bodyType = BodyType::STATIC;

	CRigidbody(float mass = 1.f, BodyType bodyType = BodyType::STATIC) : m_mass(mass), m_bodyType(bodyType) {};
	void SetMassAndInertia(float mass, const Vec2& size)
	{
		m_mass = mass;
		m_inverseMass = mass > 0 ? 1.0 / mass : 0.0f;

		float w = size.x;
		float h = size.y;

		float inertia = (mass * (w * w + h * h)) / 12.0f;
		m_inverseInertia = inertia > 0 ? 1.0f / inertia : 0.0f;
	}
};