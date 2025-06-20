#pragma once

#include<SFML/Graphics.hpp>
#include"../Maths/Vec2.h"

class CShape
{
protected:
	//Vec2 m_position;
	sf::Color m_fillColor = sf::Color::White;
	sf::Color m_outlineColor = sf::Color::Black;
	float m_outlineThickness = 0.f;
public:
	virtual ~CShape() = default;
	virtual std::unique_ptr<sf::Shape> CreateSFShape() const = 0;
	//void SetPosition(const Vec2& position) { m_position = position; }
	//void SetRotation(float rotation) { m_rotation = rotation; }
	void SetFillColor(const sf::Color& fillColor) { m_fillColor = fillColor;  }
	void SetOutlineColor(const sf::Color& outlineColor) { m_outlineColor = outlineColor; }
	void SetOutlineThickness(float outlineThickness) { m_outlineThickness = outlineThickness; }

	//Vec2 GetPosition() const { return m_position; }
	//float GetRotation() const { return m_rotation; }
	sf::Color GetFillColor() const { return m_fillColor; }
	sf::Color GetOutlineColor() const { return m_outlineColor; }
};

class CCircle : public CShape 
{
	float m_radius;
	//float m_rotation = 0.f;

public:
	CCircle(float radius) : m_radius(radius) {};

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() const { return m_radius; }
	//void SetRotation(float rotation) { m_rotation = rotation; }
	//float GetRotation() const { return m_rotation; }

	std::unique_ptr<sf::Shape> CreateSFShape() const override {
		auto shape = std::make_unique<sf::CircleShape>(m_radius);
		//shape->setPosition(sf::Vector2f(m_position.x, m_position.y));
		//shape->setRotation(sf::degrees(m_rotation));
		shape->setFillColor(m_fillColor);
		shape->setOutlineColor(m_outlineColor);
		shape->setOutlineThickness(m_outlineThickness);
		shape->setRadius(m_radius);
		shape->setOrigin(sf::Vector2f(m_radius, m_radius));
		return shape;
	}
};

class COrientedRectangle : public CShape 
{
	Vec2 m_size;

public:
	COrientedRectangle(const Vec2& size) : m_size(size) {};

	void SetSize(const Vec2& size) { m_size = size; }
	Vec2 GetSize() const { return m_size; }

	std::unique_ptr<sf::Shape> CreateSFShape() const override {
		auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_size.x, m_size.y));
		//shape->setPosition(sf::Vector2f(m_position.x, m_position.y));
		shape->setFillColor(m_fillColor);
		shape->setOutlineColor(m_outlineColor);
		shape->setOutlineThickness(m_outlineThickness);
		shape->setSize(sf::Vector2f(m_size.x, m_size.y));
		shape->setOrigin(sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f));
		return shape;
	}
};

class CPolygon : public CShape
{
	std::vector<Vec2> m_points;
	//float m_rotation = 0.f;

public:
	CPolygon(const std::vector<Vec2>& points) : m_points(points) {};

	void SetPoints(const std::vector<Vec2>& points) { m_points = m_points; }
	const std::vector<Vec2>& GetPoints() const { return m_points; }
	//void SetRotation(float rotation) { m_rotation = rotation; }
	//float GetRotation() const { return m_rotation; }

	std::unique_ptr<sf::Shape> CreateSFShape() const override {
		auto shape = std::make_unique<sf::ConvexShape>(m_points.size());
		for (size_t i = 0; i < m_points.size(); ++i) {
			shape->setPoint(i, sf::Vector2f(m_points[i].x, m_points[i].y));
		}
		//shape->setPosition(sf::Vector2f(m_position.x, m_position.y));
		//shape->setRotation(sf::degrees(m_rotation));
		shape->setFillColor(m_fillColor);
		shape->setOutlineColor(m_outlineColor);
		shape->setOutlineThickness(m_outlineThickness);
		shape->setOrigin(sf::Vector2f(0.f, 0.f)); // à ajuster si tu veux centrer
		return shape;
	}
};