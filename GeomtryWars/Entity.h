#pragma once
#include <memory>
#include "CTransform.h"
#include "CName.h"
#include "CShape.h"
#include "CBBox.h"
#include "CWeapon.h"


class Entity {
	friend class EntityManager;
	const size_t		m_id = 0;
	const std::string	m_tag = "Default";
	bool				m_alive = true;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CBBox> cBBox;
	std::shared_ptr<CWeapon> cWeapon;
	int                 m_lifeTime = 0;
	void destroy() {
		m_alive = false;
	}
	const std::string& tag() { return m_tag; }
private:
	Entity(const std::string& tag,size_t id): m_tag(tag),m_id(id)
	{}
};