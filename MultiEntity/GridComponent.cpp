#include "GridComponent.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "Actor.h"
#include "Game.h"
#include "DebugManager.h"

GridComponent::GridComponent(Actor* owner):
	Component(owner)
{
	for (int i = 0; i < gridSizeX ;i++) {
		for (int j = 0; j < gridSizeY; j++) {
			Node* newNode= new Node();
			
			newNode->nodeX = i;
			newNode->nodeY = j;

			for (int k = 0; k < nodeGridSize; k++) {
				for (int l = 0; l < nodeGridSize; l++) {

					Tile* newTile = new Tile();
					newTile->x = i*nodeGridSize + k;
					newTile->y = j*nodeGridSize + l;
					newTile->state = 0;
					newNode->tileGrid[k][l] = newTile;

					tileList.push_back(newTile);

				}
			}
			
			grid[i][j] = newNode;
		}
	}
}

Node* GridComponent::getNodeAt(int x, int y)
{
	return grid[x][y];
}

std::vector<Tile*> GridComponent::getTiles()
{
	return tileList;
}

void GridComponent::update(float dt)
{
	//Get Node and Tile Hovered
	Vector2 mousePos = GetMousePosition();
	Vector2 res = Game::instance().resolution;
	float totalXGridSize = res.x / gridSizeX;
	float totalYGridSize = res.y / gridSizeY;
	currentNodeHovered = Vector2{ floorf(mousePos.x / totalXGridSize),floorf(mousePos.y / totalYGridSize) };
	currentNodeHovered = Vector2Clamp(currentNodeHovered, Vector2Zero(), Vector2{ gridSizeX-1,gridSizeY-1 });

	Vector2 mousePosInNode = Vector2Add(mousePos, Vector2{ -totalXGridSize * currentNodeHovered.x,-totalYGridSize * currentNodeHovered.y });
	mousePosInNode = Vector2Clamp(mousePosInNode, Vector2Zero(), Vector2{ totalXGridSize-1,totalYGridSize-1 });
	std::cout << " mouse x in Node : " << mousePosInNode.x << std::endl;

	

	Node* currentNode = getNodeAt(currentNodeHovered.x, currentNodeHovered.y);
	float totalXTileSize = totalXGridSize / nodeGridSize;
	float totalYTileSize = totalYGridSize / nodeGridSize;

	currentTileHovered = Vector2{ floorf(mousePosInNode.x / totalXTileSize),floorf(mousePosInNode.y / totalYTileSize) };
	Tile* currentTile = currentNode->getTileAt(currentTileHovered.x, currentTileHovered.y);

	if (IsMouseButtonDown(0)) {
		currentTile->state = 1;
		if (lastNodeSelected != currentNode) {
			lastNodeSelected = currentNode;
			currentNode->debugColor = GREEN;
			nodeSelected.push_back(currentNode);
		}
	}
	else if (IsMouseButtonDown(1)) {
		currentTile->state = 0;
	}

	if (IsMouseButtonReleased(0)) {
		while (!nodeSelected.empty()) {
			nodeSelected.back()->debugColor = RED;
			nodeSelected.back()->updateDijkstra();
			nodeSelected.pop_back();
		}
	}
	Vector2 rPos;
	for (int x = 0; x < gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			rPos = Vector2{ totalXGridSize * x ,totalYGridSize * y };
			
			for (int i = 0; i < nodeGridSize; i++) {
				for (int j = 0; j < nodeGridSize; j++) {
					DebugManager::instance().addRectangle(Vector2Add(rPos,Vector2{i*totalXTileSize,j*totalYTileSize}), Vector2{totalXTileSize,totalYTileSize},currentTile->debugColor);
				}
			}
			DebugManager::instance().addRectangle(rPos, Vector2{ totalXGridSize,totalYGridSize }, getNodeAt(x,y)->debugColor);
		}
	}
	rPos = Vector2{ totalXGridSize * currentNodeHovered.x + currentTileHovered.x * totalXTileSize ,totalYGridSize * currentNodeHovered.y + currentTileHovered.y * totalYTileSize };
	DebugManager::instance().addRectangle(rPos, Vector2{ totalXTileSize,totalYTileSize });
}

void Node::updateDijkstra()
{

}
