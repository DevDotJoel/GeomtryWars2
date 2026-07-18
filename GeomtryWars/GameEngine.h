#pragma once
#include "EntityManager.h"



class GameEngine {

	EntityManager			m_entities;
	bool					m_isRunning = false;
	sf::RenderWindow        m_window;
	sf::Clock               m_clock;
	float                   m_enemySpawnInterval = 2.0f;
	float                   m_enemySpawnTimer = 0.0f;
	void sUserInput();
	void sMovement();
	void sRender();
	void spawnPlayer();
	void spawnEnemy();
	void sRotation();
	void spawnBullet(const Vec2& pos, const Vec2& dir, float speed);
	void init();
	void sEnemies(float dt);
	void sCollision();

public:
	GameEngine() = default;
	void run();

};