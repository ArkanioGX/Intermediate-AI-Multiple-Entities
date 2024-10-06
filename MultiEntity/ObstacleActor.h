#pragma once
#include "Actor.h"
#include "TextureComponent.h"
#include "ObstacleComponent.h"

class ObstacleActor : public Actor
{
public:
	ObstacleActor();

private:
	TextureComponent* tc;
	ObstacleComponent* oc;
};

