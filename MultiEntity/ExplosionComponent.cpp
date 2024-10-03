#include "ExplosionComponent.h"
#include "Actor.h"
#include "raymath.h"
#include "TextureComponent.h"
ExplosionComponent::ExplosionComponent(class Actor* owner):
	Component(owner)
{
	rotSpeed = GetRandomValue(minMaxRotSpeed.x, minMaxRotSpeed.y);
	getOwner()->angle = GetRandomValue(0, 360);
	tc = getOwner()->getComponent<TextureComponent*>();
}

void ExplosionComponent::update(float dt)
{
	timer += dt;
	if (timer > lifetime) {
		getOwner()->killActor();
	}
	getOwner()->setScale(Vector2AddValue(getOwner()->scale, dt*16));
	getOwner()->angle = getOwner()->angle + dt * rotSpeed;

	tc->color = Fade(tc->color, 1 - (Clamp(timer / lifetime, 0, 1)));
	
}
