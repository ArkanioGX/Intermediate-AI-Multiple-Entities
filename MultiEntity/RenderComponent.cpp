#include "RenderComponent.h"
#include "Game.h"
#include "raylib.h"

RenderComponent::RenderComponent(Actor* owner):
	Component(owner)
{
	Game::instance().addRenderComponent(this);
}

RenderComponent::~RenderComponent()
{
	
	Game::instance().removeRenderComponent(this);
}

void RenderComponent::draw()
{
	Actor* own = getOwner();
	Vector2 pos = own->getPosition();
	Vector2 scale = own->getScale();
	DrawRectangle(pos.x, pos.y, scale.x, scale.y, color);
}
