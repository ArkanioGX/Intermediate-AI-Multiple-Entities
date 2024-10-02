#pragma once

class Actor;

class Component
{
public:
	Component(Actor* a);
	virtual ~Component();

	Actor* getOwner();
	virtual void update(float dt);

private:
	Actor* owner;
};

