#pragma once
#include "Component.h"
#include "GridComponent.h"
#include "raylib.h"
#include <vector>

enum BoidType {Ice = 0, Fire = 1, Water = 2};

class BoidComponent : public Component
{
public:
	BoidComponent(class Actor* owner);

	void update(float dt) override;

	void setColor(Color c);

	Vector2 getGridParent() { return gridParent; }
	void setGridParent(Vector2 gp) { gridParent = gp; }

	void setDestination(Tile* d);

private:

	std::vector<Vector2> currentPath;
	float distToPathMin = 5;

	class TextureComponent* tc;

	float flashTime = 0.1f;
	float flashAlpha = 0;
	Color boidColor;

	Vector2 forward;

	Vector2 gridParent = Vector2{ -1,-1 };

	Vector2 Separate(class BoidActor* boid);
	Vector2 FollowPath();

	float minimumDistance = 10;
	float separateIntensity = 30;

	float boidSpeed = 350;

	float followPathIntensity = 60;
};

