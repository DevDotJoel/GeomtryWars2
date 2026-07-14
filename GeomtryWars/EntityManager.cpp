#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include <memory>
#include <algorithm>
#include <iostream>
std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{

	auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
	m_entitiesToAdd.push_back(e);
	return e;
}

void EntityManager::update()
{

	for (auto &e : m_entitiesToAdd)
	{
		std::cout << e->tag()<< "\n";
		m_entities.push_back(e);
		m_entityMap[e->m_tag].push_back(e);
	}
	m_entitiesToAdd.clear();

	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::shared_ptr<Entity>& e) { return !e->m_alive; }), m_entities.end());


	for (auto& pair : m_entityMap) {
		auto& bucket = pair.second;
		bucket.erase(std::remove_if(bucket.begin(), bucket.end(), [](const std::shared_ptr<Entity>& e){ return !e->m_alive; }), bucket.end());
	}
}
const EntityVector & EntityManager::getEntities() const{

	return m_entities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag) const  {

	static const EntityVector empty;
	auto it = m_entityMap.find(tag);

	if (it != m_entityMap.end()) {
		return it->second;                  
	}
	return empty;
}