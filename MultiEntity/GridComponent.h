#pragma once
#include "Component.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>


#define inf std::numeric_limits<int>::max()

static constexpr int gridSizeX = 16;
static constexpr int gridSizeY = 9;

static constexpr int maxDPath = 100;

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

	class Node* ownerNode;

	Vector2 side = Vector2Zero();
	class tGroup* currentGroup;
};

struct tGroup {
	std::vector<Tile* > tiles;
	std::vector<tGroup*> tileGroupNearby;

	bool isNull() { return (tiles.size() == 0 && tileGroupNearby.size() == 0); }
};

struct dijkstraNode {
	tGroup* currentTGroup;
	int dist = inf;
	tGroup* previousTGroup = nullptr;
	int id;

	bool operator== (tGroup* dn) { return currentTGroup == dn; };
	bool operator== (const dijkstraNode& dn) { return id == dn.id; };
};


struct Node {
public:
	//Node Position
	int nodeX;
	int nodeY;

	Tile* tileGrid[nodeGridSize][nodeGridSize];
	std::vector<tGroup*> tGroups;

	Tile* getTileAt(int x, int y) { return tileGrid[x][y]; }

	Color debugColor = GRAY;

	void updateDijkstra();

	class GridComponent* owner;
};

struct AStarTile {
	bool isWalkable = false;
	float g = 0, h = 0, f = 0;
	int x, y;
	AStarTile* parent;
	Tile* current;
};

class GridComponent : public Component
{
public:

	static GridComponent* Instance()
	{
		return instance;
	}

	GridComponent(class Actor* owner);
	Node* getNodeAt(int x, int y);

	Tile* getTileAtWorldPos(Vector2 p);

	int getGridWidth() { return gridSizeX; }
	int getGridHeight() { return gridSizeY; }

	std::vector<tGroup*> getDijkstraPath(tGroup* begin, tGroup* end);
	std::vector<Tile*> getAStarPath(Tile* begin, Tile* end);
	std::vector<Vector2> getPath(Tile* begin, Tile* end);
	std::vector<Vector2> getPath();
	bool hasSamePos(AStarTile at1, AStarTile at2);
	int ifIsInListViaPos(AStarTile value, std::vector<AStarTile> list);
	int searchMin(std::vector<AStarTile> list);
	int searchMin(std::vector<std::vector<dijkstraNode>::iterator> list);

	int getInternalGridSize() { return nodeGridSize; }

	void addTGroup(tGroup* tg);
	void removeTGroup(tGroup* tg);

	std::vector<Tile*> getTiles();

	

	void update(float dt) override;

private:

	static GridComponent* instance;

	Vector2 currentTileHovered;
	Vector2 currentNodeHovered;

	Vector2 currentBoidGridBegin;
	Vector2 currentBoidGridEnd;

	Node* grid[gridSizeX][gridSizeY];
	std::vector<Tile*> tileList;

	Node* lastNodeSelected;
	std::vector<Node*> nodeSelected;

	std::vector<tGroup*> tGroupsList;

	std::vector<class BoidComponent*> bListSelected;
};

