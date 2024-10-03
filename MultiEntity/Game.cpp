#include "Game.h"
#include "raylib.h"
#include "BoidActor.h"
#include <iostream>
#include <stdlib.h>     
#include <time.h>  
#include "Assets.h"



Game::Game()
{
}

void Game::load()
{
	Assets::instance().addTexture("Images/BoidSprite.png", "Boid");
	Assets::instance().addTexture("Images/Explosion2.png", "Explosion");

	int ScreenSize = 1000;
	int boidCount = 1000;
	float PlaceOffsetX = 1920 / (boidCount+1);
	float PlaceOffsetY = 1080 / (boidCount + 1);
	for (int i = 0; i < boidCount; i++) {
		Vector2 newPos = Vector2{ float(GetRandomValue(0,1920)),float(GetRandomValue(0,1080)) };
			BoidActor* boid = new BoidActor();
			boid->setPosition(newPos);
			boid->setScale(Vector2{ 1,1 });
			boidList.push_back(boid);
	}
	
}

void Game::loop()
{
	deadActorsList.clear();
	float dt =GetFrameTime();
	for (int i = 0; i < actorsList.size(); i++) {
		actorsList[i]->update(dt);
		if (actorsList[i]->getState() == ActorState::Dead) {
			deadActorsList.push_back(actorsList[i]);
		}
	}
	
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(Color{ 10,20,75,255 });
	for (int i = 0; i < renderComponentList.size(); i++) {
		renderComponentList[i]->draw();
	}
	DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 22, RAYWHITE);
	for (BoidActor* boid : boidList) {
		//DrawCircleLinesV(boid->getPosition(), 55, GREEN);
	}
	EndDrawing();
}

void Game::close()
{
}

void Game::RemoveDeadActors()
{
	for (Actor* deadActor : deadActorsList) {
		delete deadActor;
	}
}

void Game::addActor(Actor* a)
{
	actorsList.push_back(a);
}

void Game::removeActor(Actor* a)
{
	for (int i = 0; i < actorsList.size(); i++) {
		if (actorsList[i] == a) {
			actorsList.erase(actorsList.begin() + i);
			return;
		}
	}
}

void Game::addRenderComponent(RenderComponent* rc)
{
	renderComponentList.push_back(rc);
}

void Game::removeRenderComponent(RenderComponent* rc)
{
	for (int i = 0; i < renderComponentList.size(); i++) {
		if (renderComponentList[i] == rc) {
			renderComponentList.erase(renderComponentList.begin() + i);
			return;
		}
	}
}
