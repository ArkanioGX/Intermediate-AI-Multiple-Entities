#pragma once
#include "Component.h"
#include "raylib.h"
class RenderComponent : public Component
{
public:

	RenderComponent(class Actor* owner);
	virtual ~RenderComponent();

	virtual void draw();

	Color color = RAYWHITE;
};

