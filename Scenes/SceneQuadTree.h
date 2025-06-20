#pragma once

#include "../Engine/Scene.h"

class SceneQuadTree : public Scene
{
	const int m_aabbCount = 1000;
public:
	SceneQuadTree(GameEngine* game) : Scene(game) {}

	Vec2 RandomPosition();
	Vec2 RandomScale();
	Vec2 RandomVelocity();
	void Init() override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& m_window) override;
	void ProcessInput(sf::RenderWindow& m_window) override;
	void OnEnd() override;
};