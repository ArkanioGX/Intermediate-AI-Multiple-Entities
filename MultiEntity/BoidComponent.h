#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>

class BoidComponent : public Component
{
public:
	BoidComponent(class Actor* owner);

	void update(float dt) override;

private:
	Vector2 forward;

	Vector2 Separate(class BoidActor* boid);
	Vector2 Align(const std::vector<class BoidActor*>& others);
	Vector2 Group(const std::vector<class BoidActor*>& others);

	Vector2 Bait();

	Vector2 AvoidMouse();

	float minimumDistance = 40;
	float separateIntensity = 75;

	float boidSpeed = 350;

	float maxPerceiveDistance = 75;
	float alignIntensity = 15;

	float cohesionRadius = 100;
	float groupIntensity = 5;

	float baitIntensity = 5;

	float mouseRadius = 200;
	float avoidMouseIntensity = 500;

	float maxSteer = 4;
};

