#pragma once

#include "GameEngine.h"
#include "../Scenes/SceneGravity.h"
#include "../Scenes/SceneCollision.h"

GameEngine::GameEngine() : m_window(sf::VideoMode({ 800, 600 }), "SebF2DEngine"), m_currentFrame(0)
{
	m_window.setFramerateLimit(60);
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

	m_currentScene->Init();
}

void GameEngine::Run()
{
	Init();

	while (m_running && m_window.isOpen())
	{
		m_dt = m_clock.restart().asSeconds();
		ProcessEvents();
		Update();
		Render();
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