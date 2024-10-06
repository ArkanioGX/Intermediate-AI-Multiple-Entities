#define _USE_MATH_DEFINES

#include "BoidComponent.h"
#include "BoidActor.h"
#include "Game.h"
#include "iostream"
#include "raymath.h"
#include "math.h"
#include "BoidGroupManager.h"
#include "TextureComponent.h"
#include "ExplosionActor.h"
#include "ObstacleActor.h"
#include <utility>
#include <type_traits>

Color BoidComponent::teamColor[3] = { GREEN,ORANGE,SKYBLUE };
Color BoidComponent::teamColor2[3] = { DARKGREEN,RED,BLUE };

BoidComponent::BoidComponent(Actor* owner):
	Component(owner)
{
	forward = Vector2Normalize(Vector2Subtract(getOwner()->getPosition(), Vector2{ 400,400 }));
	changeTeam(static_cast<BoidType>(GetRandomValue(0, 2)));
}

void BoidComponent::update(float dt)
{
	Vector2 gPos = BoidGroupManager::Instance()->getGridPos(getOwner()->getPosition());
	
	if (Vector2Equals(gridParent, gPos) == 0) {
		
		BoidGroupManager::Instance()->AddChild(static_cast<BoidActor*>(getOwner()), gPos);
		gridParent = gPos;
	}
	
	std::vector<class BoidActor*> boidsList = BoidGroupManager::Instance()->getBoids(gPos);

	if (flashAlpha != 0) {
		flashAlpha = Clamp(flashAlpha - (dt / flashTime), 0.f, 1.f);
		tc->color = GetColorFromFlash(boidColor, flashAlpha);
	}

	Vector2 nextMove = forward;
	Vector2 separateDir = Vector2Zero();;

	int boidAlignPerceived = 0;
	Vector2 avgForce = Vector2Zero();

	int boidGroupPerceived = 0;
	Vector2 avgPos = Vector2Zero();

	Vector2 obstacleDir = Vector2Zero();

	Vector2 huntForce = Vector2Zero();
	Vector2 fleeForce = Vector2Zero();

	for (BoidActor* boid : boidsList) {
		if (boid == getOwner()) {
			continue;
		}
		separateDir = Vector2Add(separateDir, Separate(boid));
		if (boid->getComponent<BoidComponent*>()->team == team) {
			Align(boid, avgForce, boidAlignPerceived);
			Group(boid, avgPos, boidGroupPerceived);
		}

		huntForce = Vector2Add(huntForce, Hunt(boid));
		fleeForce = Vector2Add(fleeForce, Flee(boid));
	}

	obstacleDir = AvoidObstacles();
	nextMove = Vector2Add(nextMove, Vector2Scale(obstacleDir, avoidObstacleIntensity));

	nextMove = Vector2Add(nextMove, Vector2Scale(separateDir, separateIntensity));
	boidAlignPerceived = std::max(1, boidAlignPerceived);
	nextMove = Vector2Add(nextMove, Vector2Scale(Vector2{ avgForce.x / boidAlignPerceived,avgForce.y / boidAlignPerceived }, alignIntensity));
	boidGroupPerceived = std::max(1, boidGroupPerceived);
	Vector2 forceToAvgPos = Vector2Normalize(Vector2Subtract(Vector2{ avgPos.x / boidGroupPerceived,avgPos.y / boidGroupPerceived }, getOwner()->getPosition()));
	nextMove = Vector2Add(nextMove, Vector2Scale(forceToAvgPos, groupIntensity));
	nextMove = Vector2Add(nextMove, Vector2Scale(huntForce, huntIntensity));
	nextMove = Vector2Add(nextMove, Vector2Scale(fleeForce, fleeIntensity));

	//v2 = Bait();
	//nextMove = Vector2Add(nextMove, Vector2Scale(v2, baitIntensity));

	if (IsMouseButtonDown(0)) {
		Vector2 v2 = AvoidMouse();
		nextMove = Vector2Add(nextMove, Vector2Scale(v2, avoidMouseIntensity));
	}

	nextMove = Vector2Normalize(nextMove);


	
	float angleSpeed = Clamp(Vector2Angle(forward, nextMove), -maxSteer * dt, maxSteer * dt);
	forward = Vector2Rotate(forward,angleSpeed);
	Vector2 currentPos = getOwner()->pos;
	Vector2 nextPos = Vector2Add(Vector2Scale(forward,boidSpeed*dt),currentPos);
	getOwner()->pos = nextPos;

	getOwner()->angle = Vector2Angle(Vector2{ 1,0 }, forward)*180/M_PI;
}

void BoidComponent::changeTeam(BoidType bt)
{
	team = bt;
	flashAlpha = 1;
	float colorMix = Clamp(float(GetRandomValue(0, 1000)) / 1000,0,1);
	boidColor = ColorLerp(teamColor[BoidType(bt)], teamColor2[BoidType(bt)],colorMix) ;
	tc = getOwner()->getComponent<TextureComponent*>();
	tc->color = GetColorFromFlash(boidColor,flashAlpha);
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

Vector2 BoidComponent::Hunt(BoidActor* boid)
{
	if (team != (boid->getComponent<BoidComponent*>()->team + 1) % 3)
		return Vector2Zero();
	Vector2 boidPos = getOwner()->pos;
	Vector2 force = Vector2Zero();
	float dist = Vector2Distance(boidPos, boid->pos);
	if ( dist < huntRadius) {
		force = Vector2Normalize(Vector2Subtract(boid->pos, boidPos));
		if (dist < transformRange) {
			Transform(boid);
			getOwner()->setScale(Vector2Scale(getOwner()->getScale(), 1.002));
			
		}
	}
	return force;
}

Vector2 BoidComponent::Flee(BoidActor* boid)
{
	if (team != (boid->getComponent<BoidComponent*>()->team - 1) % 3)
		return Vector2Zero();
	Vector2 force = { 0,0 };
	Vector2 boidPos = getOwner()->pos;
	if (Vector2Distance(boidPos, boid->pos) < fleeRadius) {
		force = Vector2Normalize(Vector2Subtract(boidPos, boid->pos));
	}
	return force;
}

Vector2 BoidComponent::AvoidObstacles()
{
	Vector2 force = Vector2Zero();
	std::vector<ObstacleActor*> obsList = Game::instance().obstacleList;
	for (ObstacleActor* obstacle : obsList) {
		Vector2 minPos = Vector2Subtract(obstacle->pos, Vector2Scale(obstacle->scale, 8));
		Vector2 scale = Vector2Scale(obstacle->scale, 16);
		Vector2 boidPos = getOwner()->pos;
		//Border Detection
		//AABB Detection :)
		if (boidPos.x > minPos.x - ObstacleRange &&
			boidPos.x < minPos.x + scale.x + ObstacleRange &&
			boidPos.y > minPos.y - ObstacleRange &&
			boidPos.y < minPos.y + scale.y + ObstacleRange) {
			if (boidPos.x < minPos.x) {
				force = { force.x - 1, force.y };
			}
			if (boidPos.y < minPos.y) {
				force = { force.x, force.y - 1 };
			}
			if (boidPos.x > minPos.x + scale.x ) {
				force = { force.x + 1, force.y };
			}
			if (boidPos.y > minPos.y + scale.y) {
				force = { force.x, force.y + 1 };
			}
		}
		
	}
	return Vector2Normalize(force);
}

Color BoidComponent::GetColorFromFlash(Color col, float alpha)
{
	Vector4 colV4 = ColorNormalize(col);
	Vector3 colV3 = Vector3{ colV4.x,colV4.y,colV4.z };
	colV3 = Vector3Lerp(colV3, Vector3One(), alpha);
	colV4 = Vector4{ colV3.x,colV3.y,colV3.z,colV4.w };
	return ColorFromNormalized(colV4);
}

Color BoidComponent::ColorLerp(Color col1, Color col2, float alpha)
{
	Vector4 col1V4 = ColorNormalize(col1);
	Vector3 col1V3 = Vector3{ col1V4.x,col1V4.y,col1V4.z };
	Vector4 col2V4 = ColorNormalize(col2);
	Vector3 col2V3 = Vector3{ col2V4.x,col2V4.y,col2V4.z };
	Vector3 finalColor = Vector3Lerp(col1V3, col2V3, alpha);
	Vector4 finalColV4 = Vector4{ finalColor.x,finalColor.y,finalColor.z,col1V4.w };
	return ColorFromNormalized(finalColV4);
}

void BoidComponent::Transform(BoidActor* boid)
{
	//if (boid->getComponent<BoidComponent*>()->team != team) {
		//ExplosionActor* ea = new ExplosionActor(getOwner()->pos);
	//}
	boid->getComponent<BoidComponent*>()->changeTeam(team);
	
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



