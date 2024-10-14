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
	int w = gc->getGridWidth();
	int h = gc->getGridHeight();

	float nodeW = float(Game::instance().resolution.x) / w;
	float nodeH = float(Game::instance().resolution.y) / h;

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			Node* currentNode = gc->getNodeAt(x, y);

			if (currentNode->state == 1) {
				Vector2 pos = Vector2Multiply(Vector2{ float(x),float(y) }, Vector2{ nodeW,nodeH });
				DrawRectangle(pos.x, pos.y, nodeW, nodeH, BLACK);
			}
			
		}
	}
}
