#define _USE_MATH_DEFINES

#include "BoidComponent.h"
#include "BoidActor.h"
#include "Game.h"
#include "iostream"
#include "raymath.h"
#include "math.h"
#include "BoidGroupManager.h"
#include "TextureComponent.h"
#include "GridComponent.h"
#include "DebugManager.h"

#include <utility>
#include <type_traits>

BoidComponent::BoidComponent(Actor* owner):
	Component(owner)
{
	forward = Vector2Normalize(Vector2Subtract(getOwner()->getPosition(), Vector2{ 400,400 }));
	tc = getOwner()->getComponent<TextureComponent*>();
	//currentPath.clear();
	//currentPath = GridComponent::Instance()->getPath();
}

void BoidComponent::update(float dt)
{
	Vector2 gPos = BoidGroupManager::Instance()->getGridPos(getOwner()->getPosition());
	
	if (Vector2Equals(gridParent, gPos) == 0) {
		
		BoidGroupManager::Instance()->AddChild(static_cast<BoidActor*>(getOwner()), gPos);
		gridParent = gPos;
	}
	
	std::vector<class BoidActor*> boidsList = BoidGroupManager::Instance()->getBoids(gPos);

	Vector2 nextMove = forward;
	Vector2 separateDir = Vector2Zero();;

	Vector2 pathDir = Vector2Zero();

	for (BoidActor* boid : boidsList) {
		if (boid == getOwner()) {
			continue;
		}
		separateDir = Vector2Add(separateDir, Separate(boid));

	}
	nextMove = Vector2Add(nextMove, Vector2Scale(separateDir, separateIntensity));
	pathDir = FollowPath();
	nextMove = Vector2Add(nextMove, Vector2Scale(pathDir, followPathIntensity));
	
	
	//boidAlignPerceived = std::max(1, boidAlignPerceived);
	//nextMove = Vector2Add(nextMove, Vector2Scale(Vector2{ avgForce.x / boidAlignPerceived,avgForce.y / boidAlignPerceived }, alignIntensity));
	//boidGroupPerceived = std::max(1, boidGroupPerceived);
	//Vector2 forceToAvgPos = Vector2Normalize(Vector2Subtract(Vector2{ avgPos.x / boidGroupPerceived,avgPos.y / boidGroupPerceived }, getOwner()->getPosition()));
	//nextMove = Vector2Add(nextMove, Vector2Scale(forceToAvgPos, groupIntensity));

	nextMove = Vector2Normalize(nextMove);


	
	float angleSpeed = Clamp(Vector2Angle(forward, nextMove), -10000, 10000);
	forward = Vector2Rotate(forward,angleSpeed);
	Vector2 currentPos = getOwner()->pos;
	Vector2 nextPos = Vector2Add(Vector2Scale(forward,boidSpeed*dt),currentPos);
	getOwner()->pos = nextPos;

	getOwner()->angle = Vector2Angle(Vector2{ 1,0 }, forward)*180/M_PI;
}

void BoidComponent::setColor(Color c)
{
	tc->color = c;
}

void BoidComponent::setDestination(Tile* d)
{
	Tile* b = GridComponent::Instance()->getTileAtWorldPos(getOwner()->getPosition());
	currentPath = GridComponent::Instance()->getPath(b, d);
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

Vector2 BoidComponent::FollowPath()
{
	if (currentPath.size() == 0) currentPath.push_back(getOwner()->pos);

	if (Vector2Distance(getOwner()->getPosition(), currentPath.back()) < distToPathMin) {
		if (currentPath.size() != 1) {
			currentPath.pop_back();
		}
		
	}

	for (int i = 0; i < currentPath.size() - 1; i++) {
		DebugManager::instance().addLine(currentPath[i], currentPath[i + 1], GOLD, false);
	}

	return Vector2Normalize(Vector2Subtract(currentPath.back(), getOwner()->pos));;
}




