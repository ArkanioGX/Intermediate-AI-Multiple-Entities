#include "Actor.h"

#include "Game.h"

int Actor::NextActorID = 0;

Actor::Actor()
{
	Game::instance().addActor(this);
	actorID = NextActorID;
	NextActorID++;
}

void Actor::update(float dt)
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->update(dt);
	}
}

bool Actor::addComponent(Component* c)
{
	Components.push_back(c);
	return true;
}

Game* Actor::getGame()
{
	return &Game::instance();
	//return Game::game ;
}

Vector2 Actor::getPosition()
{
	return pos;
}

Vector2 Actor::getScale()
{
	return scale;
}

void Actor::setPosition(Vector2 posT)
{
	pos = posT;
}

void Actor::setScale(Vector2 sclT)
{
	scale = sclT;
}


bool Actor::operator==(const Actor& act2)
{
	return actorID == act2.actorID;
}
