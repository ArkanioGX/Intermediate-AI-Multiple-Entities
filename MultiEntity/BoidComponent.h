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
	Vector2 forward = { 100,100 };

	Vector2 Separate(const std::vector<class BoidActor*>& others);
	Vector2 Align(const std::vector<class BoidActor*>& others);

	float minimumDistance = 20;
	float separateIntensity = 10;

	float maxPerceiveDistance = 150;
};

