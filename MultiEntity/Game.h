#pragma once
#include "Actor.h"
#include "RenderComponent.h"
#include <vector>

class Game
{
	std::vector<Actor*> actorsList;
	std::vector<Actor*> deadActorsList;
	std::vector<RenderComponent*> renderComponentList;

	Texture background ;

public:

	static Game& instance() {
		static Game inst;
		return inst;
	}

	Game();

	Vector2 resolution;

	void load();
	void loop();
	void draw();
	void close();
	void RemoveDeadActors();

	void addActor(Actor*);
	void removeActor(Actor*);

	void addRenderComponent(RenderComponent*);
	void removeRenderComponent(RenderComponent*);
};

