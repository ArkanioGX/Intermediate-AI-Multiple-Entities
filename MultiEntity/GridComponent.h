#pragma once
#include "Component.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>

static constexpr int gridSizeX = 10;
static constexpr int gridSizeY = 10;

static constexpr int nodeGridSize = 8;

static constexpr Color colorDebugGroup[5] = { GREEN,YELLOW,BLUE,GOLD,VIOLET };



struct Tile {
	//position
	int x;
	int y;

	int localX;
	int localY;

	//state : 0 = noNodeLinked, 1 = Obstacle, 2... Dijkstra Group
	int state = 0;

	Color debugColor = RED;

	bool operator == (const Tile& t) {
		return (x == t.x) && (y == t.x);
	}

	Vector2 side = Vector2Zero();
	class tileGroup* currentGroup;
};

struct tileGroup {
	std::vector<Tile* > tiles;
	std::vector<DijkstraNode*> dNodes;
};

struct DijkstraLink {
	class DijkstraNode* node1;
	class DijkstraNode* node2;

	int dist;
};

struct DijkstraNode {
	std::vector<DijkstraLink*> links;
	class Node* node1;
	class Node* node2;
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

	std::vector<DijkstraNode*> DNodeList;
};

