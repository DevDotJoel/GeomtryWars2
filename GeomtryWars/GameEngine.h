#pragma once
#include "EntityManager.h"
#include <random>


class GameEngine {

	EntityManager			m_entities;
	bool					m_isRunning = false;
	sf::RenderWindow        m_window;
	sf::Clock               m_clock;
	float                   m_enemySpawnInterval = 2.0f;
	float                   m_enemySpawnTimer = 0.0f;
	int                     m_maxEnemies=15;
	const int               m_maxSpecialAttackCycle = 4;
	float                   m_specialCooldown = 10.0f;
	float                   m_currentCooldown = 0.0f;
	const float             m_specialCycleInterval = 0.75f;
	int                     m_specialCyclesLeft = 0;
	bool                    m_specialAttackStarted = false;
	bool                    m_canShoot = false;
	std::mt19937            m_rng{ std::random_device{}() };
	void sUserInput(float dt);
	void sMovement();
	void sRender();
	void spawnPlayer();
	void spawnEnemy();
	void playerSpecialAttack(const Entity& player);
	void sRotation();
	void spawnBullet(const Vec2& pos, const Vec2& dir, float speed);
	void init();
	void sEnemies(float dt);
	void sCollision();
	void sLifetime(float dt);
public:
	GameEngine() = default;
	void run();

};