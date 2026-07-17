#include "GameEngine.h"
#include <iostream>

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
	player->cTransform = std::make_shared<CTransform>(Vec2(400, 300), Vec2(0, 0), 0.0f, 15.0f);
	player->cShape = std::make_shared<CShape>(32, 8, sf::Color::Blue, sf::Color::White);
	player->cBBox = std::make_shared<CBBox>(32, 32);
	player->cName = std::make_shared<CName>("Player1");
	player->cWeapon = std::make_shared<CWeapon>(0.1f, 0.1f, 35.0f);
}
void GameEngine::spawnEnemy()
{
	auto enemy = m_entities.addEntity("enemy");
	enemy->cTransform = std::make_shared<CTransform>(Vec2(400, 300), Vec2(0, 0), 0.0f, 15.0f);
	enemy->cShape = std::make_shared<CShape>(32, 8, sf::Color::Blue, sf::Color::White);
	enemy->cBBox = std::make_shared<CBBox>(32, 32);
	enemy->cName = std::make_shared<CName>("Player1");
	enemy->cWeapon = std::make_shared<CWeapon>(0.1f, 0.1f, 35.0f);
}

void GameEngine::sRotation()
{
	for (auto &e : m_entities.getEntities("player"))
	{
		if (!e->cTransform)
			continue;

		e->cTransform->angle += 60.0f * (1.0f / 60.0f);
	}
}
void GameEngine::sUserInput()
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
	}
	for (auto &e : m_entities.getEntities("player"))
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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && e->cWeapon->cooldown <= 0)
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
	}
}
void GameEngine::spawnBullet(const Vec2 &pos, const Vec2 &dir, float speed)
{
	auto bullet = m_entities.addEntity("bullet");
	bullet->cTransform = std::make_shared<CTransform>(pos, dir, 0.0f, speed);
	bullet->cShape = std::make_shared<CShape>(5, 8, sf::Color::Red, sf::Color::White);
	bullet->cBBox = std::make_shared<CBBox>(10, 10);
}

void GameEngine::sMovement()
{
	for (auto &e : m_entities.getEntities())
	{
		if (!e->cTransform)
			continue;
		e->cTransform->pos += e->cTransform->velocity * e->cTransform->speed;
		const auto win = m_window.getSize();
		if (e->cBBox && e->tag() != "bullet")
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

void GameEngine::run()
{

	init();

	while (m_isRunning)
	{
		m_entities.update();
		sRotation();
		sUserInput();
		sMovement();
		sRender();
	}
}
