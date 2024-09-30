#define _USE_MATH_DEFINES

#include "BoidComponent.h"
#include "BoidActor.h"
#include "Game.h"
#include "iostream"
#include "raymath.h"
#include "math.h"

BoidComponent::BoidComponent(Actor* owner):
	Component(owner)
{
	forward = Vector2Normalize(Vector2Subtract(getOwner()->getPosition(), Vector2{ 400,400 }));
}

void BoidComponent::update(float dt)
{
	Vector2 nextMove = forward;
	Vector2 v2 = Separate(getOwner()->getGame()->boidList);
	nextMove = Vector2Add(nextMove, Vector2Scale(v2,separateIntensity));

	v2 = Align(getOwner()->getGame()->boidList);
	nextMove = Vector2Add(nextMove, Vector2Scale(v2,alignIntensity));

	v2 = Group(getOwner()->getGame()->boidList);
	nextMove = Vector2Add(nextMove, Vector2Scale(v2, groupIntensity));

	//v2 = Bait();
	//nextMove = Vector2Add(nextMove, Vector2Scale(v2, baitIntensity));

	v2 = AvoidMouse();
	nextMove = Vector2Add(nextMove, Vector2Scale(v2, avoidMouseIntensity));

	nextMove = Vector2Normalize(nextMove);


	
	float angleSpeed = Clamp(Vector2Angle(forward, nextMove), -maxSteer * dt, maxSteer * dt);
	forward = Vector2Rotate(forward,angleSpeed);
	Vector2 currentPos = getOwner()->pos;
	Vector2 nextPos = Vector2Add(Vector2Scale(forward,boidSpeed*dt),currentPos);
	getOwner()->pos = nextPos;

	getOwner()->angle = Vector2Angle(Vector2{ 1,0 }, forward)*180/M_PI;
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

		float distance = Vector2Distance(boidPos, otherBoidPos);

		if (distance < minimumDistance) {
			force = Vector2Add(Vector2Normalize(boidVector),force);
		}
	}
	//Border Detection
	if (boidPos.x < minimumDistance) {
		force = { force.x + 1, force.y };
	}
	if (boidPos.y < minimumDistance) {
		force = { force.x, force.y + 1 };
	}

	if (boidPos.x + minimumDistance > 1920) {
		force = { force.x - 1, force.y };
	}
	if (boidPos.y + minimumDistance > 1080) {
		force = { force.x, force.y - 1 };
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

		float distance = Vector2Distance(boidPos, otherBoidPos);

		if (distance < maxPerceiveDistance) {
			BoidComponent* othComp = boid->getComponent<BoidComponent*>();
			force = Vector2Add(force,othComp->forward);
			boidPerceived++;
		}
	}
	if (boidPerceived == 0) { return Vector2{ 0,0 }; }
	return Vector2{force.x/boidPerceived,force.y/boidPerceived};
}

Vector2 BoidComponent::Group(const std::vector<class BoidActor*>& others)
{
	Vector2 avgPos = { 0,0 };
	int boidPerceived = 0;
	Vector2 boidPos = getOwner()->pos;
	for (BoidActor* boid : others) {
		if (boid == getOwner()) {
			continue;
		}
		Vector2 otherBoidPos = boid->pos;

		float distance = Vector2Distance(boidPos, otherBoidPos);

		if (distance < cohesionRadius) {
			avgPos = Vector2Add(avgPos, otherBoidPos);
			boidPerceived++;
		}
	}
	if (boidPerceived == 0) { return Vector2{ 0,0 }; }
	avgPos = Vector2{ avgPos.x / boidPerceived,avgPos.y / boidPerceived };
	Vector2 force = Vector2Normalize(Vector2Subtract(avgPos, boidPos));
	return force;
}

Vector2 BoidComponent::Bait()
{
	Vector2 boidPos = getOwner()->pos;
	Vector2 force = Vector2Normalize(Vector2Subtract(GetMousePosition(), boidPos));
	return force;
}

Vector2 BoidComponent::AvoidMouse()
{
	Vector2 force = { 0,0 };
	Vector2 boidPos = getOwner()->pos;
	if (Vector2Distance(boidPos, GetMousePosition()) < mouseRadius) {
		force = Vector2Normalize(Vector2Subtract(boidPos, GetMousePosition()));
	}
	return force;
}



