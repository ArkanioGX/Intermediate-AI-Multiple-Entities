#pragma once
#include "RenderComponent.h"
#include "raylib.h"
class TextureComponent : public RenderComponent
{
public:
	Texture2D tex;

	TextureComponent(class Actor* owner, const char* imgPath = "");

	void draw() override;
};

