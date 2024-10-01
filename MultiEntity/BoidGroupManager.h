#pragma once
#include "BoidActor.h";
class BoidGroupManager
{
public:
	static BoidGroupManager* Instance();

	Vector2 getGridPos(Vector2 worldPos);

	bool AddChild(BoidActor*,Vector2 newPos);

	std::vector<BoidActor*> getBoids(Vector2 gridPos);

private:
	int cellNeighborCheck = 2;

	int cellSizeX = 128;
	int cellSizeY = 108;

	Vector2 minGrid = Vector2{ 0,0 };
	Vector2 maxGrid = Vector2{ float(floor(1920 / cellSizeX)),float(floor(1080 / cellSizeX)) };

	BoidGroupManager();
	static BoidGroupManager* instance;

	std::vector<std::vector<std::vector<BoidActor*>>> GridContent;

};

