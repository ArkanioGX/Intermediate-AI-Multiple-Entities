#define _USE_MATH_DEFINES

#include "BoidComponent.h"
#include "BoidActor.h"
#include "Game.h"
#include "iostream"
#include "raymath.h"
#include "math.h"
#include "BoidGroupManager.h"
#include "TextureComponent.h"

BoidComponent::BoidComponent(Actor* owner):
	Component(owner)
{
	forward = Vector2Normalize(Vector2Subtract(getOwner()->getPosition(), Vector2{ 400,400 }));
}

void BoidComponent::update(float dt)
{
	if (getOwner()->getActorID() == 5) {
		getOwner()->getComponent<TextureComponent*>()->color = GREEN;
		Vector2 gPos = BoidGroupManager::Instance()->getGridPos(getOwner()->getPosition());
		//std::cout << " x : " << gPos.x << " | y : " << gPos.y << std::endl;
		//std::cout << (Vector2Equals(gridParent, gPos) == 0  ? "New Grid !" : "Same Grid") << std::endl;
		if (Vector2Equals(gridParent, gPos) == 0) {
			BoidGroupManager::Instance()->AddChild(static_cast<BoidActor*>(getOwner()), gPos);
		}
		gridParent = gPos;
	}


	Vector2 nextMove = forward;
	Vector2 separateDir = Vector2Zero();;
	std::vector<class BoidActor*>& boidsList = Game::instance().boidList;

	int boidAlignPerceived = 0;
	Vector2 avgForce = Vector2Zero();

	int boidGroupPerceived = 0;
	Vector2 avgPos = Vector2Zero();

	for (BoidActor* boid : boidsList) {
		if (boid == getOwner()) {
			continue;
		}
		separateDir = Vector2Add(separateDir, Separate(boid));
		Align(boid,avgForce,boidAlignPerceived);
		Group(boid,avgPos,boidGroupPerceived);
	}

	nextMove = Vector2Add(nextMove, Vector2Scale(separateDir, separateIntensity));
	boidAlignPerceived = std::max(1, boidAlignPerceived);
	nextMove = Vector2Add(nextMove, Vector2Scale(Vector2{ avgForce.x / boidAlignPerceived,avgForce.y / boidAlignPerceived }, alignIntensity));
	boidGroupPerceived = std::max(1, boidGroupPerceived);
	Vector2 forceToAvgPos = Vector2Normalize(Vector2Subtract(Vector2{ avgPos.x / boidGroupPerceived,avgPos.y / boidGroupPerceived }, getOwner()->getPosition()));
	nextMove = Vector2Add(nextMove, Vector2Scale(forceToAvgPos, groupIntensity));

	//v2 = Bait();
	//nextMove = Vector2Add(nextMove, Vector2Scale(v2, baitIntensity));

	Vector2 v2 = AvoidMouse();
	nextMove = Vector2Add(nextMove, Vector2Scale(v2, avoidMouseIntensity));

	nextMove = Vector2Normalize(nextMove);


	
	float angleSpeed = Clamp(Vector2Angle(forward, nextMove), -maxSteer * dt, maxSteer * dt);
	forward = Vector2Rotate(forward,angleSpeed);
	Vector2 currentPos = getOwner()->pos;
	Vector2 nextPos = Vector2Add(Vector2Scale(forward,boidSpeed*dt),currentPos);
	getOwner()->pos = nextPos;

	getOwner()->angle = Vector2Angle(Vector2{ 1,0 }, forward)*180/M_PI;
}

Vector2 BoidComponent::Separate(BoidActor* boid)
{

	Vector2 force = Vector2Zero();
	Vector2 boidPos = getOwner()->pos;

	Vector2 otherBoidPos = boid->pos;

	Vector2 boidVector{ boidPos.x - otherBoidPos.x,boidPos.y - otherBoidPos.y };

	float distance = Vector2Distance(boidPos, otherBoidPos);

	if (distance < minimumDistance) {
		force = Vector2Normalize(boidVector);
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

void BoidComponent::Align(BoidActor* boid, Vector2& avgForce, int& boidPerceived)
{
	Vector2 boidPos = getOwner()->pos;

		Vector2 otherBoidPos = boid->pos;

		float distance = Vector2Distance(boidPos, otherBoidPos);

		if (distance < maxPerceiveDistance) {
			BoidComponent* othComp = boid->getComponent<BoidComponent*>();
			avgForce = Vector2Add(avgForce,othComp->forward);
			boidPerceived++;
		}
}

void BoidComponent::Group(BoidActor* boid, Vector2& avgPos, int& boidPerceived)
{
	Vector2 boidPos = getOwner()->pos;
	Vector2 otherBoidPos = boid->pos;

	float distance = Vector2Distance(boidPos, otherBoidPos);

	if (distance < cohesionRadius) {
		avgPos = Vector2Add(avgPos, otherBoidPos);
		boidPerceived++;
	}
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



