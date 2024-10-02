#include "ExplosionComponent.h"
#include "Actor.h"
#include "raymath.h"

ExplosionComponent::ExplosionComponent(class Actor* owner):
	Component(owner)
{
	rotSpeed = GetRandomValue(minMaxRotSpeed.x, minMaxRotSpeed.y);
	getOwner()->angle = GetRandomValue(0, 360);
}

void ExplosionComponent::update(float dt)
{
	timer += dt;
	if (timer > lifetime) {
		getOwner()->killActor();
	}
	getOwner()->setScale(Vector2AddValue(getOwner()->scale, dt*3));
	//getOwner()->angle = getOwner()->angle + dt * rotSpeed;
}
