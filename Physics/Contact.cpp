#pragma once
#include "Contact.h"
#include "../Game/CRigidbody.h"

void Contact::ResolvePenetration()
{
	auto& aRigidbody = aEntity->cRigidbody;
	auto& bRigidbody = bEntity->cRigidbody;

	float aInvMass = aRigidbody->m_inverseMass;
	float bInvMass = bRigidbody->m_inverseMass;
	if (aRigidbody->m_bodyType == CRigidbody::BodyType::STATIC)
	{
		aInvMass = 0.0f;
	}
	else if (bRigidbody->m_bodyType == CRigidbody::BodyType::STATIC)
	{
		bInvMass = 0.0f;
	}
	float da = depth / (aInvMass + bInvMass) * aInvMass;
	float db = depth / (aInvMass + bInvMass) * bInvMass;

	aEntity->cTransform->m_position -= normal * da;
	bEntity->cTransform->m_position += normal * db;
}

void Contact::ResolveCollision()
{
	ResolvePenetration();

	auto& aRigidbody = aEntity->cRigidbody;
	auto& bRigidbody = bEntity->cRigidbody;

	auto& aTransform = aEntity->cTransform;
	auto& bTransform = bEntity->cTransform;

	float e = std::min(aRigidbody->m_bounce, bRigidbody->m_bounce);

	Vec2 ra = end - aTransform->m_position;
	Vec2 rb = start - bTransform->m_position;
	Vec2 va = aRigidbody->m_velocity + Vec2(-aRigidbody->m_angularVelocity * ra.y, aRigidbody->m_angularVelocity * ra.x);
	Vec2 vb = bRigidbody->m_velocity + Vec2(-bRigidbody->m_angularVelocity * rb.y, bRigidbody->m_angularVelocity * rb.x);
	const Vec2 vrel = va - vb;

	float vrelDotNormal = Vec2::Dot(vrel, normal);

	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * vrelDotNormal / ((aRigidbody->m_inverseMass + bRigidbody->m_inverseMass) + Vec2::Cross(ra, normal) * Vec2::Cross(ra, normal) * aRigidbody->m_inverseInertia + Vec2::Cross(rb, normal) * Vec2::Cross(rb, normal) * bRigidbody->m_inverseInertia);

	Vec2 j = impulseDirection * impulseMagnitude;

	aRigidbody->ApplyImpulse(j, ra);
	bRigidbody->ApplyImpulse(-j, rb);
}