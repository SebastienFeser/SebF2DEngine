#pragma once

#include "../Engine/Scene.h"

class SceneFallingCircle : public Scene
{
private:
	sf::Texture basketballTexture;
	sf::Sprite* basketballSprite;
public:
	SceneFallingCircle(GameEngine* game) : Scene(game) {
		basketballTexture.loadFromFile("../../../Assets/Sprites/BasketBall.png");
		basketballSprite = new sf::Sprite(basketballTexture);
	}

	~SceneFallingCircle()
	{
		delete basketballSprite;
	}

	void Init() override;
	void Update(float dt) override;
	void Render(sf::RenderWindow& m_window) override;
	void ProcessInput(sf::RenderWindow& m_window) override;
	void OnEnd() override;
};