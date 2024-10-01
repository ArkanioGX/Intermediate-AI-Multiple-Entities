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
	int cellSizeX = 128;
	int cellSizeY = 108;
	BoidGroupManager();
	static BoidGroupManager* instance;

	std::vector<std::vector<std::vector<BoidActor*>>> GridContent;

};

