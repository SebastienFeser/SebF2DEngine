#include "QuadTree.h"

QuadTree::QuadTree(int level, const Vec2& position, const Vec2& size) : m_level(level), m_position(position), m_size(size) {}

void QuadTree::Clear()
{
	m_entities.clear();
	for (auto& child : m_children)
	{
		if (child)
		{
			child->Clear();
			child.reset();
		}
	}
}

void QuadTree::Split()
{
	Vec2 half = m_size * 0.5f;
	Vec2 center = m_position;

	m_children[0] = std::make_unique<QuadTree>(m_level + 1, center + Vec2(half.x * 0.5f, -half.y * 0.5f), half);
	m_children[1] = std::make_unique<QuadTree>(m_level + 1, center + Vec2(-half.x * 0.5f, -half.y * 0.5f), half);
	m_children[2] = std::make_unique<QuadTree>(m_level + 1, center + Vec2(-half.x * 0.5f, half.y * 0.5f), half);
	m_children[3] = std::make_unique<QuadTree>(m_level + 1, center + Vec2(half.x * 0.5f, half.y * 0.5f), half);
}

int QuadTree::GetIndex(const std::shared_ptr<Entity>& entity) const
{
	if (!entity->cTransform || !entity->cCollider) return -1;
	if (entity->cCollider->m_type != ColliderType::AABB) return -1;

	const Vec2& pos = entity->cTransform->m_position;
	const Vec2& size = static_cast<CAABBCollider*>(entity->cCollider.get())->m_size;
	Vec2 center = m_position;

	bool top = pos.y + size.y < center.y;
	bool bottom = pos.y > center.y;
	bool left = pos.x + size.x < center.x;
	bool right = pos.x > center.x;
	if (top)
	{
		if (right) return 0;
		if (left) return 1;
	}
	if (bottom)
	{
		if (left) return 2;
		if (right) return 3;
	}
	return -1;
}

void QuadTree::Insert(std::shared_ptr<Entity> entity)
{
	if (m_children[0])
	{
		int index = GetIndex(entity);
		if (index != -1)
		{
			m_children[index]->Insert(entity);
			return;
		}
	}

	m_entities.push_back(entity);

	if (m_entities.size() > MAX_OBJECTS && m_level < MAX_LEVELS)
	{
		if (!m_children[0]) Split();
		for (auto entity = m_entities.begin(); entity != m_entities.end();)
		{
			int index = GetIndex(*entity);
			if (index != -1)
			{
				m_children[index]->Insert(*entity);
				entity = m_entities.erase(entity);
			}
			else
			{
				++entity;
			}
		}
	}
}

void QuadTree::Retrieve(std::vector<std::shared_ptr<Entity>>& returnEntities, const std::shared_ptr<Entity>& target)
{
	int index = GetIndex(target);
	if (index != -1 && m_children[0])
	{
		m_children[index]->Retrieve(returnEntities, target);
	}

	returnEntities.insert(returnEntities.end(), m_entities.begin(), m_entities.end());
}