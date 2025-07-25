#pragma once

#include "GameEngine.h"
#include "../Scenes/SceneGravity.h"
#include "../Scenes/SceneCollision.h"
#include "../Scenes/SceneNoQuadTree.h"
#include "../Scenes/SceneQuadTree.h"
#include "../Scenes/SceneFalling.h"
#include "../Config//GameConfig.h"
#include "../Scenes/SceneFallingCircle.h"
#include <iostream>

GameEngine::GameEngine() : m_window(sf::VideoMode({ GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT }), "SebF2DEngine"), m_currentFrame(0)
{
	//m_window.setFramerateLimit(60);
}

void GameEngine::Init()
{
	ChangeScene("SceneGravity");

	/*auto circle = m_entityManager.AddEntity("Circle");
	circle->cTransform = std::make_shared <CTransform>(Vec2(100, 100));
	circle->cShape = std::make_shared<CCircle>(50.0f);
	circle->cRigidbody = std::make_shared<CRigidbody>(1.0f, false);
	circle->cRigidbody->m_velocity = Vec2(5, 5);*/
}

void GameEngine::ChangeScene(const std::string& name)
{
	if (name == "SceneGravity")
	{
		m_currentScene = std::make_shared<SceneGravity>(this);
	}
	else if (name == "SceneCollision")
	{
		m_currentScene = std::make_shared<SceneCollision>(this);
	}
	else if (name == "SceneNoQuadTree")
	{
		m_currentScene = std::make_shared<SceneNoQuadTree>(this);
	}
	else if (name == "SceneQuadTree")
	{
		m_currentScene = std::make_shared<SceneQuadTree>(this);
	}
	else if (name == "SceneFalling")
	{
		m_currentScene = std::make_shared<SceneFalling>(this);
	}
	else if (name == "SceneFallingCircle")
	{
		m_currentScene = std::make_shared<SceneFallingCircle>(this);
	}

	m_currentScene->Init();
}

void GameEngine::Run()
{
	Init();
	sf::Font font;
	if (!font.openFromFile("../../../Assets/Fonts/ARIAL.TTF"))
	{
		std::cout << "Cant load FPS font" << std::endl;
	}

	sf::Text fpsText(font);
	fpsText.setCharacterSize(24);
	fpsText.setFillColor(sf::Color::White);
	fpsText.setPosition(sf::Vector2f(10, 10));
	//fpsText.setStyle(sf::Text::Bold);
	int fps = 0;
	int frameCount = 0;
	float accumulatedTime = 0;
	sf::Clock displayClock;

	while (m_running && m_window.isOpen())
	{
		//TODO: Stop the clock when paused
		accumulatedTime += m_dt;
		m_dt = m_clock.restart().asSeconds();
		frameCount++;
		if (displayClock.getElapsedTime().asSeconds() >= 0.2f && accumulatedTime != 0)
		{
			fps = frameCount / accumulatedTime;
			frameCount = 0;
			accumulatedTime = 0.f;
			displayClock.restart();
		}

		fpsText.setString("FPS: " + std::to_string(fps));
		ProcessEvents();
		Update();
		Render();

		m_window.draw(fpsText);
		m_window.display();
		m_currentFrame++;
	}
}

void GameEngine::ProcessEvents()
{
	if (m_currentScene)
	{
		m_currentScene->ProcessInput(m_window);
	}
}

void GameEngine::Update()
{
	if(m_currentScene) 
	{
		m_currentScene->Update(m_dt);
	}
	/*m_entityManager.Update();
	m_physics.Update(m_entityManager, m_dt);
	//Handle Inputs
	//Handle Movement
	//Handle Collisions*/
}

void GameEngine::Render()
{
	if (m_currentScene)
	{
		m_currentScene->Render(m_window);
	}
	/*
	m_window.clear();

	for (auto& e : m_entityManager.GetEntities())
	{
		if (e->cShape && e->cTransform)
		{
			auto shape = e->cShape->CreateSFShape();
			shape->setPosition(sf::Vector2f(e->cTransform->m_position.x, e->cTransform->m_position.y));
			m_window.draw(*shape);
		}
	}

	m_window.display();*/
}

/*void GameEngine::MainLoop()
{
	m_entityManager.Update();
	//Handle Inputs
	//Handle Movement
	//Handle Collisions
	//Handle Render
	m_currentFrame++;
}*/