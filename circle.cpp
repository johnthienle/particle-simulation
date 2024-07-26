#include <iostream>
#include <cmath>
#include "Circle.hpp"

Circle::Circle(float x, float y, float r, float m)
    : position(x, y), radius(r), mass(m) {
    velocity = sf::Vector2f(0, 0);
    acceleration = sf::Vector2f(0, 0);
    force = sf::Vector2f(0, 0);
    circle = sf::CircleShape(radius);
    circle.setPosition(position);
}

void Circle::draw(sf::RenderWindow &window) {
    circle.setFillColor(sf::Color::Blue);
    window.draw(circle);
}

bool Circle::checkCollision(Circle other) {
    sf::FloatRect circle1 = circle.getGlobalBounds();
    sf::FloatRect circle2 = other.circle.getGlobalBounds();
    float dx = (circle.getPosition().x + ((circle1.width) / 2) - (other.circle.getPosition().x + ((circle2.width) / 2)));
    float dy = (circle.getPosition().y + ((circle1.height) / 2) - (other.circle.getPosition().y + ((circle2.height) / 2)));

    float distance = std::sqrt((dx * dx) + (dy * dy));

    float radius1 = radius;
    float radius2 = other.radius;

    return distance <= (radius1 + radius2);
}

