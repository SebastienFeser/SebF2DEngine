#pragma once
#include "Contact.h"
#include "../Game/CRigidbody.h"

void Contact::ResolvePenertration()
{
	auto& aRigidbody = aEntity->cRigidbody;
	auto& bRigidbody = aEntity->cRigidbody;

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
	ResolvePenertration();

	auto& aRigidbody = aEntity->cRigidbody;
	auto& bRigidbody = bEntity->cRigidbody;

	float e = std::min(aRigidbody->m_bounce, bRigidbody->m_bounce);

	const Vec2 vrel = (aRigidbody->m_velocity - bRigidbody->m_velocity);

	float vrelDotNormal = Vec2::Dot(vrel, normal);

	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * vrelDotNormal / (aRigidbody->m_inverseMass + bRigidbody->m_inverseMass);

	Vec2 j = impulseDirection * impulseMagnitude;

	aRigidbody->ApplyImpulse(j);
	bRigidbody->ApplyImpulse(-j);
}