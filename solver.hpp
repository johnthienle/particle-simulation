#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "verletCircle.hpp"

using namespace sf;

struct Solver {

	std::vector<verletCircle> circles;
	Vector2f constraintCenter;
	float constraintRadius = 100.0f;
	float frameDt = (1.0f / 60.0f);
	uint32_t subSteps = 8;

	Vector2f gravity = { 0.0f, 1000.0f };

	[[nodiscard]]
	const std::vector<verletCircle>& getObjects() const {
		return circles;
	}

	verletCircle& addObject(Vector2f position, float radius) {
		circles.emplace_back(position, radius);
		return circles.back();
	}

	[[nodiscard]]
	sf::Vector3f getConstraint() const {
		return { constraintCenter.x, constraintCenter.y, constraintRadius };
	}

	int getObjectCount() {
		return circles.size();
	}

	void update(float dt) {
		const float subDt = frameDt / float(subSteps);
		for (uint32_t i = subSteps; i--;) {
			applyGravity();
			checkCollisions(subDt);
			applyConstraint();
			updatePositions(subDt);
		}
	}

	void updatePositions(float dt) {
		for (auto& circle : circles) {
			circle.updatePosition(dt);
		}
	}

	void applyGravity() {
		for (auto& circle : circles) {
			circle.accelerate(gravity);
		}
	}

	void setConstraint(sf::Vector2f position, float radius) {
		constraintCenter = position;
		constraintRadius = radius;
	}

	void setSubStepsCount(uint32_t sub_steps) {
		subSteps = sub_steps;
	}

	void applyConstraint() {
		for (auto& circle : circles) {
			const sf::Vector2f v = constraintCenter - circle.position;
			const float dist = sqrt(v.x * v.x + v.y * v.y);
			if (dist > (constraintRadius - circle.radius)) {
				const sf::Vector2f n = v / dist;
				circle.position = constraintCenter - n * (constraintRadius - circle.radius);
			}
		}
	}

	void checkCollisions(float dt) {
		const float coef = 0.75f;
		const uint64_t circleCount = circles.size();
		for (uint64_t i = 0; i < circleCount; ++i) {
			verletCircle& circle1 = circles[i];
			for (uint64_t j = i + 1; j < circleCount; ++j) {
				verletCircle& circle2 = circles[j];
				const Vector2f v = circle1.position - circle2.position;
				const float dist2 = v.x * v.x + v.y * v.y;
				const float minDist = circle1.radius + circle2.radius;

				if (dist2 < minDist * minDist) {
					const float dist = sqrt(dist2);;
					const Vector2f n = v / dist;
					const float massRatio1 = circle1.radius / (circle1.radius + circle2.radius);
					const float massRatio2 = circle2.radius / (circle1.radius + circle2.radius);
					const float delta = 0.5f * coef * (dist - minDist);
					circle1.position -= n * (massRatio2 * delta);
					circle2.position += n * (massRatio1 * delta);
				}
			}
		}
	}
};

