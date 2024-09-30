#include "RenderComponent.h"
#include "Game.h"
#include "raylib.h"

RenderComponent::RenderComponent(Actor* owner):
	Component(owner)
{
	Game::instance().addRenderComponent(this);
}

void RenderComponent::draw()
{
	Actor* own = getOwner();
	Vector2 pos = own->getPosition();
	Vector2 scale = own->getScale();
	DrawRectangle(pos.x, pos.y, scale.x, scale.y, Color{ 255,0,255,255 });
}
