#include "Component.h"
#include "Actor.h"

Component::Component(Actor* a)
{
	owner = a;
	a->addComponent(this);
}

Actor* Component::getOwner()
{
	return owner;
}

void Component::update(float dt)
{
}
