#pragma once

#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) : m_tag(tag), m_id(id) {};

std::string Entity::GetTag() const
{
	return m_tag;
}

bool Entity::GetAlive() const
{
	return m_alive;
}