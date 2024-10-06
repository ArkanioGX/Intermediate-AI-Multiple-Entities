#pragma once
#include "Actor.h"
#include "RenderComponent.h"
#include <vector>

class Game
{
	std::vector<Actor*> actorsList;
	std::vector<Actor*> deadActorsList;
	std::vector<RenderComponent*> renderComponentList;
public:

	static Game& instance() {
		static Game inst;
		return inst;
	}

	Game();

	void load();
	void loop();
	void draw();
	void close();
	void RemoveDeadActors();

	void addActor(Actor*);
	void removeActor(Actor*);

	void addRenderComponent(RenderComponent*);
	void removeRenderComponent(RenderComponent*);

	void addObstacle(class ObstacleActor*);

	std::vector<class BoidActor*> boidList;
	std::vector<class ObstacleActor*> obstacleList;

	Vector2 obstacleStartPos = Vector2{ 0,0 };
};

