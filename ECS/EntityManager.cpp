#pragma once

#include "EntityManager.h"

std::shared_ptr<Entity> EntityManager::AddEntity(const std::string&  tag)
{
	auto e = std::make_shared<Entity>(tag, m_totalEntities++);
	m_toAdd.push_back(e);
	return e;
}

EntityManager::EntityManager() : m_totalEntities(0) {}

void EntityManager::Update()
{
	for (auto e : m_toAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->GetTag()].push_back(e);
	}

	m_entities.erase(
		std::remove_if(m_entities.begin(), m_entities.end(), 
			[](const std::shared_ptr<Entity>& e) {
				return !e->GetAlive(); 
			}), 
		m_entities.end()
	);

	for (auto& [tag, vec] : m_entityMap)
	{
		vec.erase(
			std::remove_if(vec.begin(), vec.end(),
				[](const std::shared_ptr<Entity>& e) {
					return !e->GetAlive();
				}),
			vec.end()
		);
	}


	m_toAdd.clear();
}

EntityVec& EntityManager::GetEntities()
{
	return m_entities;
}

EntityVec& EntityManager::GetEntities(const std::string& tag)
{
	return m_entityMap[tag];
}