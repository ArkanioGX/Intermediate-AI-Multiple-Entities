#include "TextureComponent.h"
#include "Actor.h"
#include "raymath.h"
#include "Assets.h"


TextureComponent::TextureComponent(Actor* owner, Texture2D& texT):
	RenderComponent(owner),
	tex(texT)
{
	color = WHITE;
}

void TextureComponent::draw()
{
	Actor* own = getOwner();
	Vector2 pos = own->getPosition();
	Vector2 scale = own->getScale();
	float angle = own->angle;
	
	int display = GetCurrentMonitor();

	DrawTextureEx(tex, Vector2Subtract(pos, Vector2{ float(tex.width),float(tex.height) }), angle, scale.x,color);
}

