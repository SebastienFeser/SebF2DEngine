#pragma once
#include "../Maths/Vec2.h"


class CTransform
{
public:
	Vec2 m_position = { 0,0 };
	float m_angle = 0.f;

	CTransform() {}
	CTransform(const Vec2& position) : m_position(position), m_angle(0){};
	CTransform(const Vec2& position, float angle) : m_position(position), m_angle(angle) {};
};