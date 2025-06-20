#pragma once
#include <vector>
#include <memory>
#include "../Maths/Vec2.h"
#include "../ECS/Entity.h"


class QuadTree
{
public: 
	QuadTree(int level, const Vec2& position, const Vec2& size);

	void Clear();
	void Insert(std::shared_ptr<Entity> entity);
	void Retrieve(std::vector<std::shared_ptr<Entity>>& returnEntities, const std::shared_ptr<Entity>& target);
	void Draw(sf::RenderWindow& m_window);

private:
	void Split();
	int GetIndex(const std::shared_ptr<Entity>& entity) const;

	Vec2 m_position;
	Vec2 m_size;
	int m_level;
	const int MAX_OBJECTS = 10;
	const int MAX_LEVELS = 6;

	std::vector<std::shared_ptr<Entity>> m_entities;
	std::unique_ptr<QuadTree> m_children[4];
};