#pragma once
#include "Vec2.h"

class CTransform {

public:
	Vec2 pos = { 0,0 };
	Vec2 velocity = { 0,0 };
	float angle = 0.0f;
	float speed = 0.0f;

	CTransform(){}
	CTransform(const Vec2& p, const Vec2& v, float a = 0.0f,float speedin=0.0f)
		: pos(p), velocity(v), angle(a), speed(speedin) {
	}

};