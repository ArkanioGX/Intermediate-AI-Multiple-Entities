#pragma once
#include "RenderComponent.h"
#include "raylib.h"
class TextureComponent : public RenderComponent
{
public:
	Texture2D& tex;

	TextureComponent(class Actor* owner, Texture2D& texT);
	~TextureComponent();

	void draw() override;
};

