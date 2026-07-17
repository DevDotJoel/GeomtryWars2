#pragma once
#include "EntityManager.h"



class GameEngine {

	EntityManager			m_entities;
	bool					m_isRunning = false;
	sf::RenderWindow        m_window;
	void sUserInput();
	void sMovement();
	void sRender();
	void spawnPlayer();
	void spawnEnemy();
	void sRotation();
	void spawnBullet(const Vec2& pos, const Vec2& dir, float speed);
	void init();

public:
	GameEngine() = default;
	void run();

};