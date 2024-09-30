#pragma once
#include <vector>
#include "Component.h"
#include "raylib.h"

class Actor
{
protected:
	std::vector<Component*> Components;

public:
	Vector2 pos = Vector2{ 0,0 };
	Vector2 scale = Vector2{ 100,100 };
	float angle = 0;

public:
	Actor();

	void update(float dt);
	bool addComponent(Component*);
	bool removeComponent(Component*);

	template <typename CompType>
	inline CompType getComponent()
	{
		for (Component* currComp : Components)
		{
			CompType currentEntry = dynamic_cast<CompType>(currComp);
			if (currentEntry != nullptr)
			{
				return currentEntry;
			}
		}
		return nullptr;
	}

	class Game* getGame();

	Vector2 getPosition();
	Vector2 getScale();

	void setPosition(Vector2 posT);
	void setScale(Vector2 sclT);

	bool operator==(const Actor& act2);
	

private:
	static int NextActorID;
	int actorID;
};

