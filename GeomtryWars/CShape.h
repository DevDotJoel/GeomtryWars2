#pragma once
#include <SFML/Graphics.hpp>

class CShape {

public:
	sf::CircleShape shape;
public:
	CShape(){}

    CShape(float radius, size_t points, const sf::Color& fill, const sf::Color& outline)
        : shape(radius, points)
    {
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(-2.0f);   
        shape.setOrigin(radius, radius);   
    }
};