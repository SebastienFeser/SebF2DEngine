#pragma once

#include"../ECS/EntityManager.h"
#include <SFML/Graphics.hpp>
#include "../Physics/SPhysics.h"

class Scene;

class GameEngine {
	sf::RenderWindow m_window;
	EntityManager m_entityManager;
	size_t m_currentFrame;
	SPhysics m_physics;
	std::shared_ptr<Scene> m_currentScene;

	sf::Clock m_clock;
	float m_dt = 0.f;
	bool m_running = true;

	void Init();
	void ProcessEvents();
	void Update();
	void Render();
public:

	void ChangeScene(const std::string& name);
	GameEngine();
	void Run();
};