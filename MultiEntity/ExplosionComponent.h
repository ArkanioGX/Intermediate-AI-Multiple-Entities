#pragma once
#include "Component.h"
#include "raylib.h"
class ExplosionComponent: public Component
{
private:
	float lifetime = 0.2f;
	float timer = 0;

	float rotSpeed = 5;
	Vector2 minMaxRotSpeed = Vector2{ 180,360 };
public:
	ExplosionComponent(class Actor* owner);
	class TextureComponent* tc;

	void update(float dt) override;
};

