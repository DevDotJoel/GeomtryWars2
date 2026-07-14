#include "Vec2.h"

Vec2 Vec2::operator +(const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}
Vec2 Vec2::operator *(float value) const {

	return Vec2(x *value, y * value);
}
void Vec2:: operator +=(const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}
Vec2& Vec2::add(const Vec2& v) {
	x += v.x;
	y += v.y;

	return *this;
}
Vec2& Vec2::rotate(float deg) {

	return *this;
}

Vec2& Vec2::scale(float s) {

	x *= s;
	y *= s;
	return *this;
}

Vec2 Vec2::operator -(const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y); 
}

float Vec2::dist(const Vec2& v) const {

	return (*this - v).length();
}
float Vec2::length() const { return sqrtf(x * x + y * y); }

Vec2 Vec2::normalized() const {
	float len = length();
	return len > 0 ? Vec2(x / len, y / len) : Vec2(0, 0);
}