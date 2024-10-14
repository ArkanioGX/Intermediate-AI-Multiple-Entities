#pragma once
#include "Actor.h"


class GridActor : public Actor
{
public:
	GridActor();
private:
	class GridComponent* gc;
	class GridRendererComponent* gr;
};

