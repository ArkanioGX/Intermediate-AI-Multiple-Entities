#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <stdlib.h>     
#include <time.h>  
#include "Assets.h"
#include "raymath.h"
#include "GridActor.h"
#include "DebugManager.h"



Game::Game()
{
}

void Game::load()
{
	resolution = Vector2{ float(GetRenderWidth()),float(GetRenderHeight()) };
	Assets::addTexture("Images/background.png", "background");
	background = Assets::getTexture("background");

	GridActor* ga = new GridActor();
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
	ClearBackground(Color{ 5,10,25,255 });
	if (background.id != 0) {
		Vector2 pos = Vector2Zero();
		Rectangle src = Rectangle{ 0,0,float(background.width),float(background.height) };
		Rectangle dest = Rectangle{ 0,0,resolution.x,resolution.y };
		Vector2 origin = Vector2Zero();
		DrawTexturePro(background, src, dest, origin, 0, WHITE);
	}
	for (int i = 0; i < renderComponentList.size(); i++) {
		renderComponentList[i]->draw();
	}
	DebugManager::instance().draw();
	DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 22, RAYWHITE);
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
