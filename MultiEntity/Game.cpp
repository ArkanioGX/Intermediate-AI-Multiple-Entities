#include "Game.h"
#include "raylib.h"
#include "BoidActor.h"
#include <iostream>



Game::Game()
{
}

void Game::load()
{
	int ScreenSize = 800;
	int boidCount = 5;
	float PlaceOffset = float(800) / (boidCount+1);
	for (int i = 0; i < boidCount; i++) {
		for (int j = 0; j < boidCount; j++) {
			BoidActor* boid = new BoidActor();
			boid->setPosition(Vector2{ PlaceOffset * (i + 1),PlaceOffset * (j + 1) });
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
