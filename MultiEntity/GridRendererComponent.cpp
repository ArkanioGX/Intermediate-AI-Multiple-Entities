#include "GridRendererComponent.h"
#include "Actor.h"
#include "GridComponent.h"
#include "Game.h"
#include "raymath.h"

GridRendererComponent::GridRendererComponent(Actor* owner, GridComponent* gcT):
	RenderComponent(owner),
	gc(gcT)
{
}

GridRendererComponent::~GridRendererComponent()
{
	Game::instance().removeRenderComponent(this);
}

void GridRendererComponent::draw()
{
	int w = gc->getGridWidth() * gc->getInternalGridSize();
	int h = gc->getGridHeight()* gc->getInternalGridSize();

	float nodeW = float(Game::instance().resolution.x) / w;
	float nodeH = float(Game::instance().resolution.y) / h;

	std::vector<Tile*> list = gc->getTiles();

	for (Tile* t: list) {
			if (t->state != 0) {
				Vector2 pos = Vector2Multiply(Vector2{ float(t->x),float(t->y) }, Vector2{ nodeW,nodeH });
				DrawRectangle(pos.x, pos.y, nodeW, nodeH, TileColor[t->state]);
			}
			
	}
}
