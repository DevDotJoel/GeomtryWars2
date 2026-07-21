#include "GameEngine.h"
#include <iostream>
#include <random>
void GameEngine::init()
{

	m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
	m_window.setFramerateLimit(60);
	spawnPlayer();
	m_isRunning = true;
}

void GameEngine::spawnPlayer()
{

	auto player = m_entities.addEntity("player");
	player->cTransform = std::make_shared<CTransform>(Vec2(400, 300), Vec2(0, 0), 0.0f, 15.0f, 60.0f);
	player->cShape = std::make_shared<CShape>(32, 8, sf::Color::Blue, sf::Color::White);
	player->cBBox = std::make_shared<CBBox>(64, 64);
	player->cName = std::make_shared<CName>("Player1");
	player->cWeapon = std::make_shared<CWeapon>(0.1f, 0.1f, 15.0f);
}
void GameEngine::spawnEnemy()
{
	const auto win = m_window.getSize();
	const float r = 16.0f; //radius of each enemy

	// random float for the positions
	auto randf = [&](float lo, float hi)
		{
			std::uniform_real_distribution<float> d(lo, hi);
			return d(m_rng);
		};
	//edges

	std::uniform_int_distribution<int> edgeDist(0, 3);
	int edge = edgeDist(m_rng);

	Vec2 pos(0, 0);
	Vec2 vel(0, 0);

	switch (edge)
	{
	case 0:
		pos.x = randf(r, (float)win.x - r);
		pos.y = r;
		vel.x = randf(-1.0f, 1.0f);
		vel.y = 1.0f;
		break;
	case 1:
		pos.x =(float)win.x - r;
		pos.y = randf(r, (float)win.y - r);
		vel.x =-1.0f ;
		vel.y = randf(-1.0f, 1.0f);
		break;
	case 2:
		pos.x =r;
		pos.y = (float)win.y - r;
		vel.x = 1.0f;
		vel.y = randf(-1.0f, 1.0f);
		break;
	case 3:
		pos.x = r;
		pos.y = randf(r, (float)win.y - r);
		vel.x = 1.0f;
		vel.y = randf(-1.0f, 1.0f);
		break;

	default:
		break;
	}

	if (vel.length() > 0)
		vel=vel.normalized();

	auto enemy = m_entities.addEntity("enemy");
	enemy->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f, 3.0f, 0.0f);
	enemy->cShape = std::make_shared<CShape>(32, 8, sf::Color(randf(0,255), randf(0, 255), randf(0, 255)), sf::Color(randf(0, 255), randf(0, 255), randf(0, 255)));
	enemy->cBBox = std::make_shared<CBBox>(32, 32);
	enemy->cName = std::make_shared<CName>("Enemy");
}

void GameEngine::sRotation()
{
	for (auto &e : m_entities.getEntities("player"))
	{
		if (!e->cTransform)
			continue;

		e->cTransform->angle += e->cTransform->rotSpeed * (1.0f / 60.0f);
	}
}
void GameEngine::sUserInput(float dt)
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_isRunning = false;

		if (event.type == sf::Event::Resized)
		{

			sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
			m_window.setView(sf::View(visibleArea));
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			if (m_specialCyclesLeft == 0 && !m_specialAttackStarted && m_currentCooldown <=0.0f)
			{
				m_specialCyclesLeft = m_maxSpecialAttackCycle;   // kick off N cycles
				m_currentCooldown = 0.0f;
				m_specialAttackStarted = true;
			}
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
		{
			m_pauseGame = !m_pauseGame;
		}
	}


	if (!m_pauseGame) {
		m_currentCooldown -= dt;
		for (auto& e : m_entities.getEntities("player"))
		{
			if (!e->cTransform || !e->cWeapon)
				continue;
			Vec2 dir(0, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				dir.y -= 1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				dir.y += 1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				dir.x -= 1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				dir.x += 1;
			if (dir.length() > 0)
				dir = dir.normalized();
			e->cTransform->velocity = dir;

			e->cWeapon->cooldown -= 1.0f / 60.0f; // one frame at 60fps

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && e->cWeapon->cooldown <= 0 && !m_specialAttackStarted)
			{
				// direction = from player to mouse
				sf::Vector2i mousePixel = sf::Mouse::getPosition(m_window);
				Vec2 mousePos((float)mousePixel.x, (float)mousePixel.y);
				Vec2 aimDir = mousePos - e->cTransform->pos;
				if (aimDir.length() > 0)
					aimDir = aimDir.normalized();

				spawnBullet(e->cTransform->pos, aimDir, e->cWeapon->bulletSpeed);
				e->cWeapon->cooldown = e->cWeapon->fireRate;
			}
			if (m_specialAttackStarted && m_specialCyclesLeft > 0)
			{
				if (e->cTransform && e->cWeapon && m_currentCooldown <= 0.0f) {
					playerSpecialAttack(*e);                   // one 4-way burst

					m_specialCyclesLeft -= 1;
					m_currentCooldown = m_specialCycleInterval;       // wait before next burst

					if (m_specialCyclesLeft == 0) {
						m_specialAttackStarted = false;
						m_currentCooldown = m_specialCooldown;
					}

				}

			}

		}
	}

}


void GameEngine::playerSpecialAttack(const Entity& player)
{
	Vec2 rightBulletPosition(player.cTransform->pos.x + 16, player.cTransform->pos.y);
	Vec2 rightBulletDir(1,0);

	Vec2 leftBulletPosition(player.cTransform->pos.x - 16, player.cTransform->pos.y);
	Vec2 leftBulletDir(-1, 0);

	Vec2 topBulletPosition(player.cTransform->pos.x, player.cTransform->pos.y + 16);
	Vec2 topBulletDir(0, 1);

	Vec2 bottomBulletPosition(player.cTransform->pos.x, player.cTransform->pos.y-16);
	Vec2 bottomBulletDir(0, -1);

	spawnBullet(rightBulletPosition, rightBulletDir, 20.0f);
	spawnBullet(leftBulletPosition, leftBulletDir, 20.0f);
	spawnBullet(topBulletPosition, topBulletDir, 20.0f);
	spawnBullet(bottomBulletPosition, bottomBulletDir, 20.0f);
	
	
	

}
void GameEngine::spawnBullet(const Vec2 &pos, const Vec2 &dir, float speed)
{
	auto bullet = m_entities.addEntity("bullet");
	bullet->cTransform = std::make_shared<CTransform>(pos, dir, 0.0f, speed);
	bullet->cShape = std::make_shared<CShape>(5, 8, sf::Color::Red, sf::Color::White);
	bullet->cBBox = std::make_shared<CBBox>(10, 10);
	bullet->cLifetime = std::make_shared<CLifetime>(1.0f);
}

void GameEngine::sMovement()
{
	for (auto &e : m_entities.getEntities())
	{
		if (!e->cTransform)
			continue;
		e->cTransform->pos += e->cTransform->velocity * e->cTransform->speed;
		const auto win = m_window.getSize();
		if (e->cBBox && e->tag() == "player")
		{
			const float r = e->cBBox->width / 2.0f;
			if (e->cTransform->pos.x < r)
				e->cTransform->pos.x = r;
			if (e->cTransform->pos.x > win.x - r)
				e->cTransform->pos.x = win.x - r;
			if (e->cTransform->pos.y < r)
				e->cTransform->pos.y = r;
			if (e->cTransform->pos.y > win.y - r)
				e->cTransform->pos.y = win.y - r;
		}
		if (e->cBBox && e->tag() == "enemy")
		{
			const float r = e->cBBox->width / 2.0f;
			if (e->cTransform->pos.x - r < 0)
			{
				e->cTransform->velocity.x = -e->cTransform->velocity.x;
				e->cTransform->pos.x = r;
			}
			else if (e->cTransform->pos.x + r > win.x)
			{
				e->cTransform->velocity.x = -e->cTransform->velocity.x;
				e->cTransform->pos.x = win.x - r;
			}

			if (e->cTransform->pos.y - r < 0)
			{
				e->cTransform->velocity.y = -e->cTransform->velocity.y;
				e->cTransform->pos.y = r;
			}
			else if (e->cTransform->pos.y + r > win.y)
			{
				e->cTransform->velocity.y = -e->cTransform->velocity.y;
				e->cTransform->pos.y = win.y - r;
			}
		}
		if (e->tag() == "bullet")
		{
			const float r = e->cBBox->width / 2.0f;

			if (e->cTransform->pos.x < r || e->cTransform->pos.x > win.x + r || e->cTransform->pos.y < r || e->cTransform->pos.y > win.y + r)
			{
				e->destroy();
			}
		}
	}
}
void GameEngine::sCollision()
{
	for (auto &bullet : m_entities.getEntities("bullet"))
	{

		for (auto &enemey : m_entities.getEntities("enemy"))
		{
			float bulletRadius = bullet->cBBox->width / 2;
			float enemyRadius = enemey->cBBox->width / 2;

			float bulletLeft = bullet->cTransform->pos.x - bulletRadius;
			float bulletRight = bullet->cTransform->pos.x + bulletRadius;
			float bulletTop = bullet->cTransform->pos.y - bulletRadius;
			float bulletBottom = bullet->cTransform->pos.y + bulletRadius;

			float enemyLeft = enemey->cTransform->pos.x - enemyRadius;
			float enemyRight = enemey->cTransform->pos.x + enemyRadius;
			float enemyTop = enemey->cTransform->pos.y - enemyRadius;
			float enemyBottom = enemey->cTransform->pos.y + enemyRadius;

			if ((bulletLeft < enemyRight) && (bulletRight > enemyLeft) && (bulletTop < enemyBottom) && (bulletBottom > enemyTop))
			{
				spawnEnemy();
				enemey->destroy();
			}
		}
	}
}
void GameEngine::sEnemies(float dt)
{
	auto currentSize = m_entities.getEntities("enemy").size();
	std::cout << currentSize;
	m_enemySpawnTimer += dt;
	if (currentSize<m_maxEnemies  && m_enemySpawnTimer > m_enemySpawnInterval)
	{
		spawnEnemy();
		m_enemySpawnTimer = 0.0f;
	}
}
void GameEngine::sRender()
{
	m_window.clear(sf::Color::Black);
	for (auto &e : m_entities.getEntities())
	{
		if (!e->cShape || !e->cTransform)
			continue;
		e->cShape->shape.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cShape->shape.setRotation(e->cTransform->angle);
		m_window.draw(e->cShape->shape);
	}
	m_window.display();
}
void GameEngine::sLifetime(float dt)
{
	for (auto& e : m_entities.getEntities())
	{
		if (!e->cLifetime)
			continue;

		e->cLifetime->lifetime -= dt;

		if (e->cLifetime->lifetime <= 0)
		{
			e->destroy();
		}
	}
}
void GameEngine::run()
{

	init();
	float m_dt = 0.0f;
	while (m_isRunning)
	{
		m_dt = m_clock.restart().asSeconds();
		m_entities.update();
		sUserInput(m_dt);
		if (!m_pauseGame) {

			sEnemies(m_dt);
			sRotation();      // or fold into sMovement
			sMovement();
			sCollision();
			sLifetime(m_dt);

		}

		sRender();
	}
}
