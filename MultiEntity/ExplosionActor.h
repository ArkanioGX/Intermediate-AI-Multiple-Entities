#pragma once
#include "Actor.h"
#include "raylib.h"
#include "TextureComponent.h"
#include "ExplosionComponent.h"

class ExplosionActor : public Actor
{
	TextureComponent* tc;
	ExplosionComponent* ec;
public:
	ExplosionActor(Vector2 pos);

	
};

