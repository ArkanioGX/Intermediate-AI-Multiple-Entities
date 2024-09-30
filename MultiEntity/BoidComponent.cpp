#include "BoidComponent.h"
#include "BoidActor.h"
#include "Game.h"
#include "iostream"

BoidComponent::BoidComponent(Actor* owner):
	Component(owner)
{
}

void BoidComponent::update(float dt)
{
	Vector2 nextMove = forward;
	Vector2 v2 = Separate(getOwner()->getGame()->boidList);
	nextMove = { nextMove.x + v2.x,nextMove.y + v2.y };

	

	forward = nextMove;
	Vector2 currentPos = getOwner()->pos;
	Vector2 nextPos = Vector2{ currentPos.x + nextMove.x*dt, currentPos.y + nextMove.y*dt };
	getOwner()->pos = nextPos;


}

Vector2 BoidComponent::Separate(const std::vector<class BoidActor*>& others)
{
	Vector2 force = { 0,0 };
	Vector2 boidPos = getOwner()->pos;
	for (BoidActor* boid : others) {
		if (boid == getOwner()) {
			continue;
		}
		Vector2 otherBoidPos = boid->pos;

		Vector2 boidVector{ boidPos.x - otherBoidPos.x,boidPos.y - otherBoidPos.y };

		float distance = sqrt(pow(boidVector.x, 2) + pow(boidVector.y, 2));

		if (distance < minimumDistance) {
			float intensity = (distance / minimumDistance) * separateIntensity;

			Vector2 forceToApply = Vector2{boidVector.x/distance,boidVector.y/distance};
			force = { force.x + forceToApply.x * separateIntensity,force.y + forceToApply.y*separateIntensity };
		}
	}
	//Border Detection
	if (boidPos.x < minimumDistance) {
		float intensity = ((minimumDistance - boidPos.x) / minimumDistance) * separateIntensity;
		force = { force.x + intensity, force.y };
	}
	if (boidPos.y < minimumDistance) {
		float intensity = ((minimumDistance - boidPos.y) / minimumDistance) * separateIntensity;
		force = { force.x, force.y + intensity };
	}

	if (boidPos.x + minimumDistance > 800) {
		float intensity = ((boidPos.x - (800 - minimumDistance)) / minimumDistance) * separateIntensity;
		force = { force.x - intensity, force.y };
	}
	if (boidPos.y + minimumDistance > 800) {
		float intensity = ((boidPos.y - (800 - minimumDistance)) / minimumDistance) * separateIntensity;
		force = { force.x, force.y - intensity };
	}
	return force;
}

Vector2 BoidComponent::Align(const std::vector<class BoidActor*>& others)
{
	Vector2 force = { 0,0 };
	int boidPerceived = 0;
	Vector2 boidPos = getOwner()->pos;
	for (BoidActor* boid : others) {
		if (boid == getOwner()) {
			continue;
		}
		Vector2 otherBoidPos = boid->pos;

		Vector2 boidVector{ boidPos.x - otherBoidPos.x,boidPos.y - otherBoidPos.y };

		float distance = sqrt(pow(boidVector.x, 2) + pow(boidVector.y, 2));

		if (distance < minimumDistance) {
			BoidComponent* othComp = boid->getComponent<BoidComponent*>();
			force = Vector2{ force.x + othComp->forward.x, force.y + othComp->forward.y };
		}
	}
	return Vector2{ force.x / boidPerceived,force.y / boidPerceived };
}



