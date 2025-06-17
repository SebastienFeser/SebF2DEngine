#pragma once

#include <memory>
#include <vector>
#include <map>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map <std::string, EntityVec> EntityMap;


class EntityManager {
	EntityVec m_entities;
	EntityVec m_toAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities;
public:
	EntityManager();
	void Update();
	std::shared_ptr<Entity> AddEntity(const std::string& tag);
	EntityVec& GetEntities();
	EntityVec& GetEntities(const std::string& tag);

};