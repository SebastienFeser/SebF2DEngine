#pragma once

#include "../ECS/Entity.h"

struct Contact
{
	std::shared_ptr<Entity> aEntity;
	std::shared_ptr<Entity> bEntity;

	Vec2 start;
	Vec2 end;

	Vec2 normal;
	float depth;

	Contact() = default;
	~Contact() = default;

	void ResolvePenertration();
};