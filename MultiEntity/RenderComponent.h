#pragma once
#include "Component.h"
class RenderComponent : public Component
{
public:

	RenderComponent(class Actor* owner);

	virtual void draw();
};

