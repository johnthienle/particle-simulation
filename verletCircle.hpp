#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

struct verletCircle {
	Vector2f position;
	Vector2f positionLast;
	Vector2f acceleration;
	float radius = 10.0f;
	Color color = Color::Black;

	verletCircle() = default;
	verletCircle(Vector2f position_, float radius_)
		: position{ position_ }
		, positionLast{ position_ }
		, acceleration(0.0f, 0.0f)
		, radius{ radius_ }
	{}

	void updatePosition(float dt) {
		const Vector2f velocity = position - positionLast;
		positionLast = position;
		//Verlet Integration
		position = position + velocity + acceleration * dt * dt;
		acceleration = {};
	}

	void accelerate(Vector2f a) {
		acceleration += a;
	}

	void setVelocity(sf::Vector2f v, float dt) {
		positionLast = position - (v * dt);
	}

	void addVelocity(sf::Vector2f v, float dt) {
		positionLast -= v * dt;
	}

	[[nodiscard]]
	sf::Vector2f getVelocity(float dt) const {
		return (position - positionLast) / dt;
	}
};


