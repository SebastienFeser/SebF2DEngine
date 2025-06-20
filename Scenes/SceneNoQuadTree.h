#pragma once

#include "../Engine/Scene.h"

class SceneNoQuadTree : public Scene
{
	const int m_aabbCount = 1000;
public:
	SceneNoQuadTree(GameEngine* game) : Scene(game) {}

	void Init() override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& m_window) override;
	void ProcessInput(sf::RenderWindow& m_window) override;
	void OnEnd() override;
};