#pragma once

#include "GameEngine.h"

class Scene
{
protected:
	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	SPhysics m_physics;
	bool m_isPaused = false;
public:
	Scene(GameEngine* game) : m_game(game) {};
	virtual ~Scene() = default;

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
	virtual void ProcessInput(sf::RenderWindow& window) = 0;
	virtual void OnEnd() = 0;
};