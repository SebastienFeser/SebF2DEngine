#pragma once

#include <cmath>

class Vec2 {
public:
	float x = 0.f;
	float y = 0.f;

	Vec2() = default;
	Vec2(float x_, float y_) : x(x_), y(y_) {};

	Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; };
	Vec2 operator-(const Vec2& other) const { return { x - other.x, y - other.y }; };
	Vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; };
	Vec2& operator+=(const Vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	float Length() const { return sqrt(x * x + y * y); }
	float SquaredLength() const { return x * x + y * y; }
	Vec2 Normalized() const {
		float len = Length();
		return (len != 0) ? Vec2{ x / len, y / len } : Vec2{ 0, 0 };
	}
};