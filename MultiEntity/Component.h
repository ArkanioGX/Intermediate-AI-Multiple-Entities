#pragma once

class Actor;

class Component
{
public:
	Component(Actor* a);

	Actor* getOwner();
	virtual void update(float dt);

private:
	Actor* owner;
};

