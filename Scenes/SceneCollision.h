#pragma once

#include "../Engine/Scene.h"

class SceneCollision : public Scene
{
public: 
	SceneCollision(GameEngine* game) : Scene(game) {}

	void Init() override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& m_window) override;
	void ProcessInput(sf::RenderWindow& m_window) override;
	void OnEnd() override;
};