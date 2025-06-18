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
	Vec2 m_velocity = { 0.f,0.f };
	Vec2 m_force = { 0.f, 0.f };
	float m_angularVelocity = 0.f;
	float m_torque = 0.f;
	float m_friction = 0.1f;
	float m_bounce = 0.3f;
	BodyType m_bodyType = BodyType::STATIC;

	CRigidbody(float mass = 1.f, BodyType bodyType = BodyType::STATIC) : m_mass(mass), m_bodyType(bodyType) {};

};