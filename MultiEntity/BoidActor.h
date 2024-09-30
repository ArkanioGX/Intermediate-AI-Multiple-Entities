#pragma once
#include "BoidComponent.h"
#include "TextureComponent.h"
#include "Actor.h"

class BoidActor : public Actor
{
public:
	BoidActor();
private:
	BoidComponent* boidComp;
	TextureComponent* texComp;
};

