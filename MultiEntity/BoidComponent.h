#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>

enum BoidType {Ice = 0, Fire = 1, Water = 2};

class BoidComponent : public Component
{
public:
	BoidComponent(class Actor* owner);

	void update(float dt) override;

	Vector2 getGridParent() { return gridParent; }
	void setGridParent(Vector2 gp) { gridParent = gp; }

	void changeTeam(BoidType bt);

private:

	class TextureComponent* tc;

	float flashTime = 0.1f;
	float flashAlpha = 0;
	Color boidColor;

	static Color teamColor[];
	static Color teamColor2[];

	BoidType team;

	Vector2 forward;

	Vector2 gridParent = Vector2{ -1,-1 };

	Vector2 Separate(class BoidActor* boid);
	void Align(class BoidActor* boid, Vector2& avgForce, int& boidPerceived);
	void Group(class BoidActor* boid, Vector2& avgForce, int& boidPerceived);
	Vector2 Hunt(class BoidActor* boid);
	Vector2 Flee(class BoidActor* boid);
	Vector2 AvoidObstacles();

	Color GetColorFromFlash(Color col, float alpha);
	Color ColorLerp(Color col1, Color col2, float alpha);

	void Transform(class BoidActor* boid);

	Vector2 Bait();

	Vector2 AvoidMouse();

	float minimumDistance = 10;
	float separateIntensity = 100;

	float boidSpeed = 350;

	float maxPerceiveDistance = 20;
	float alignIntensity = 30;

	float cohesionRadius = 30;
	float groupIntensity = 5;

	float baitIntensity = 5;

	float mouseRadius = 400;
	float avoidMouseIntensity = 500;

	float maxSteer = 8;

	float huntRadius = 100;
	float huntIntensity = 50;

	float fleeRadius = 50;
	float fleeIntensity = 200;

	float transformRange = 30;

	float ObstacleRange = 60;
	float avoidObstacleIntensity = 1500;
};

