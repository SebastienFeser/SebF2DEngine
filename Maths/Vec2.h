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
	Vec2 operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
		
	}
	Vec2 operator/(float scalar) const { return { x / scalar, y / scalar }; };
	Vec2 operator-() const {
		return Vec2(-x, -y);
	}
	float Length() const { return sqrt(x * x + y * y); }
	float SquaredLength() const { return x * x + y * y; }
	Vec2 Normalized() const {
		float len = Length();
		return (len != 0) ? Vec2{ x / len, y / len } : Vec2{ 0, 0 };
	}
	static float Dot(const Vec2& a, const Vec2& b) {
		return a.x * b.x + a.y * b.y;
	}
	static float Cross(const Vec2& a, const Vec2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	Vec2 Normal()
	{
		return Vec2(y, -x).Normalized();
	}

	static Vec2 Rotate(const Vec2& v, float angle)
	{
		float cosA = std::cos(angle);
		float sinA = std::sin(angle);
		return Vec2(
			v.x * cosA - v.y * sinA,
			v.x * sinA + v.y * cosA
		);
	}
};