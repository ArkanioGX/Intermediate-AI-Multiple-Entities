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

	int ScreenSize = 1000;
	int boidCount = 10;
	float PlaceOffsetX = 1920 / (boidCount+1);
	float PlaceOffsetY = 1080 / (boidCount + 1);
	for (int i = 0; i < boidCount; i++) {
		for (int j = 0; j < boidCount; j++) {
			BoidActor* boid = new BoidActor();
			boid->setPosition(Vector2{float(i*PlaceOffsetX),float(j*PlaceOffsetY) });
			boid->setScale(Vector2{ 2,2 });
			boidList.push_back(boid);
		}
	}
	
}

void Game::loop()
{
	float dt = 1.f / 60;
	for (int i = 0; i < actorsList.size(); i++) {
		actorsList[i]->update(dt);
	}
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(Color{ 10,20,75,255 });
	for (int i = 0; i < renderComponentList.size(); i++) {
		renderComponentList[i]->draw();
	}
	for (BoidActor* boid : boidList) {
		//DrawCircleLinesV(boid->getPosition(), 55, GREEN);
	}
	EndDrawing();
}

void Game::close()
{
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
