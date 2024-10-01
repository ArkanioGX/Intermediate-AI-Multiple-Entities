#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>

class BoidComponent : public Component
{
public:
	BoidComponent(class Actor* owner);

	void update(float dt) override;

	Vector2 getGridParent() { return gridParent; }
	void setGridParent(Vector2 gp) { gridParent = gp; }

private:
	Vector2 forward;

	Vector2 gridParent = Vector2{ -1,-1 };

	Vector2 Separate(class BoidActor* boid);
	void Align(class BoidActor* boid, Vector2& avgForce, int& boidPerceived);
	void Group(class BoidActor* boid, Vector2& avgForce, int& boidPerceived);

	Vector2 Bait();

	Vector2 AvoidMouse();

	float minimumDistance = 50;
	float separateIntensity = 100;

	float boidSpeed = 350;

	float maxPerceiveDistance = 75;
	float alignIntensity = 30;

	float cohesionRadius = 100;
	float groupIntensity = 5;

	float baitIntensity = 5;

	float mouseRadius = 400;
	float avoidMouseIntensity = 500;

	float maxSteer = 8;
};

