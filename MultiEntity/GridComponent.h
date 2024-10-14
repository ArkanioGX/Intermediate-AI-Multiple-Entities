#pragma once
#include "Component.h"
#include <vector>

static constexpr int gridSizeX = 16;
static constexpr int gridSizeY = 9;

struct Node {
public:
	//state : 0 = Air, 1 = Obstacle
	int state = 0;

	//position
	int x;
	int y;

	std::vector<Node*> nodeLinked;
};

class GridComponent : public Component
{
public:

	GridComponent(class Actor* owner);
	Node* getNodeAt(int x, int y);
	int getGridWidth() { return gridSizeX; }
	int getGridHeight() { return gridSizeY; }

private:

	const int xSize = gridSizeX, ySize = gridSizeY;
	Node* grid[gridSizeX][gridSizeY];
	
};

