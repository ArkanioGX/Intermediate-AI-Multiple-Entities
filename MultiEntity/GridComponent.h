#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>

static constexpr int gridSizeX = 10;
static constexpr int gridSizeY = 10;

static constexpr int nodeGridSize = 8;



struct Tile {
	//position
	int x;
	int y;

	//state : 0 = Air, 1 = Obstacle
	int state = 0;

	Color debugColor = RED;

	
};

struct Node {
public:
	//Node Position
	int nodeX;
	int nodeY;

	Tile* tileGrid[nodeGridSize][nodeGridSize];
	std::vector<Node*> nodeLinked;

	Tile* getTileAt(int x, int y) { return tileGrid[x][y]; }

	Color debugColor = GRAY;

	void updateDijkstra();
};

class GridComponent : public Component
{
public:

	GridComponent(class Actor* owner);
	Node* getNodeAt(int x, int y);
	int getGridWidth() { return gridSizeX; }
	int getGridHeight() { return gridSizeY; }

	int getInternalGridSize() { return nodeGridSize; }

	std::vector<Tile*> getTiles();

	void update(float dt) override;

private:

	Vector2 currentTileHovered;
	Vector2 currentNodeHovered;

	Node* grid[gridSizeX][gridSizeY];
	std::vector<Tile*> tileList;

	Node* lastNodeSelected;
	std::vector<Node*> nodeSelected;
};

