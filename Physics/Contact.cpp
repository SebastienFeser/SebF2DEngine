#pragma once
#include "Contact.h"
#include "../Game/CRigidbody.h"

void Contact::ResolvePenertration()
{
	auto aRigidbody = aEntity->cRigidbody;
	auto bRigidbody = aEntity->cRigidbody;

	if ((aRigidbody->m_bodyType == CRigidbody::BodyType::STATIC && bRigidbody->m_bodyType == CRigidbody::BodyType::STATIC) ||
		aRigidbody->m_bodyType == CRigidbody::BodyType::KINEMATIC || bRigidbody->m_bodyType == CRigidbody::BodyType::KINEMATIC)
	{
		return;
	}

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