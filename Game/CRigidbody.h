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
	Vec2 m_velocity = { 0.f,0.f };
	Vec2 m_force = { 0.f, 0.f };
	float m_angularAcceleration = 0.f;
	float m_angularVelocity = 0.f;
	float m_angularDrag = 0.98f;
	float m_drag = 0.0f;
	float m_torque = 0.0f;
	float m_inertia = 0.0f;
	float m_inverseInertia = 0.0f;
	float m_friction = 0.7f;
	float m_bounce = 0.3f;
	BodyType m_bodyType = BodyType::STATIC;

	CRigidbody(float mass = 1.f, BodyType bodyType = BodyType::STATIC) : m_mass(mass), m_bodyType(bodyType) 
	{
		if (mass > 0)
		{
			m_inverseMass = 1.0f / mass;
		}
	};
	void SetMassAndInertia(float mass, float momentOfInertia)
	{
		m_mass = mass;
		m_inverseMass = mass > 0 ? 1.0 / mass : 0.0f;


		m_inertia = momentOfInertia * mass;
		m_inverseInertia = m_inertia > 0 ? 1.0f / m_inertia : 0.0f;
		//float w = size.x;
		//float h = size.y;

		//float inertia = (mass * (w * w + h * h)) / 12.0f;
		//m_inverseInertia = inertia > 0 ? 1.0f / inertia : 0.0f;
	}

	void AddForce(const Vec2& force)
	{
		m_force += force;
	}

	void AddTorque(float torque)
	{
		m_torque += torque;
	}

	void ClearForces()
	{
		m_force = Vec2(0, 0);
	}

	void ClearTorque()
	{
		m_torque = 0.0f;
	}

	void IntegrateLinear(float dt)
	{
		Vec2 acceleration = m_force * m_inverseMass;

		m_velocity += acceleration * dt;

		ClearForces();
	}

	void IntegrateAngular(float dt)
	{
		m_angularAcceleration = m_torque * m_inverseInertia;

		m_angularVelocity += m_angularAcceleration * dt;

		ClearTorque();
	}

	void GenerateAndApplyDragForce(float dt)
	{
		//Vec2 dragForce = Vec2(0, 0);
		if (m_velocity.SquaredLength() > 0)
		{
			Vec2 dragDirection = m_velocity.Normalized() * -1.0;

			float dragMagnitude = m_drag * m_velocity.SquaredLength();

			m_velocity += dragDirection * dragMagnitude * dt;
		}
	}

	void GenerateAndApplyFrictionForce(float dt)
	{
		Vec2 frictionDirection = m_velocity.Normalized() * -1.0f;

		m_velocity += frictionDirection * m_friction;
	}

	void ApplyImpulse(const Vec2& j)
	{
		if (m_bodyType == BodyType::STATIC)
		{
			return;
		}

		m_velocity += j * m_inverseMass;
	}

	void ApplyImpulse(const Vec2& j, const Vec2& r)
	{
		if (m_bodyType == BodyType::STATIC)
		{
			return;
		}

		m_velocity += j * m_inverseMass;
		m_angularVelocity += Vec2::Cross(r, j) * m_inverseInertia;
	}
};