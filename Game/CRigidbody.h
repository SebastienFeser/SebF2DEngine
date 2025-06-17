#pragma once
#include "../Maths/Vec2.h"

class CRigidbody {
public:
	float m_mass = 1.f;
	Vec2 m_velocity = { 0.f,0.f };
	Vec2 m_force = { 0.f, 0.f };
	float m_angularVelocity = 0.f;
	float m_torque = 0.f;
	float m_friction = 0.1f;
	float m_bounce = 0.3f;
	bool m_isStatic = false;

	CRigidbody(float mass = 1.f, bool isStatic = false) : m_mass(mass), m_isStatic(isStatic) {};

};