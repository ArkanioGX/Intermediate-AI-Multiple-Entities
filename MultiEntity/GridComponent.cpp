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
					newTile->localX = k;
					newTile->localY = l;
					newTile->state = 0;
					newNode->tileGrid[k][l] = newTile;

					if (k == 0) {
						newTile->side = Vector2Add(newTile->side, Vector2{ -1,0 });
					}
					if (k == nodeGridSize-1) {
						newTile->side = Vector2Add(newTile->side, Vector2{ 1,0 });
					}
					if (l == 0) {
						newTile->side = Vector2Add(newTile->side, Vector2{ 0,-1 });
					}
					if (l == nodeGridSize - 1) {
						newTile->side = Vector2Add(newTile->side, Vector2{ 0,1 });
					}

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
	//std::cout << " mouse x in Node : " << mousePosInNode.x << std::endl;

	

	Node* currentNode = getNodeAt(currentNodeHovered.x, currentNodeHovered.y);
	float totalXTileSize = totalXGridSize / nodeGridSize;
	float totalYTileSize = totalYGridSize / nodeGridSize;

	currentTileHovered = Vector2{ floorf(mousePosInNode.x / totalXTileSize),floorf(mousePosInNode.y / totalYTileSize) };
	Tile* currentTile = currentNode->getTileAt(currentTileHovered.x, currentTileHovered.y);

	if (IsMouseButtonDown(0) || IsMouseButtonPressed(0)) {
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
		lastNodeSelected = nullptr;
	}
	Vector2 rPos;
	for (int x = 0; x < gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			rPos = Vector2{ totalXGridSize * x ,totalYGridSize * y };
			
			for (int i = 0; i < nodeGridSize; i++) {
				for (int j = 0; j < nodeGridSize; j++) {
					DebugManager::instance().addRectangle(Vector2Add(rPos,Vector2{i*totalXTileSize,j*totalYTileSize}), Vector2{totalXTileSize,totalYTileSize}, getNodeAt(x, y)->getTileAt(i,j)->debugColor);
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
	std::vector<Tile*> tileUnchecked;
	for (int x = 0; x < nodeGridSize; x++) {
		for (int y = 0; y < nodeGridSize; y++) {
			Tile* currentTile = tileGrid[x][y];
			currentTile->debugColor = RED;
			if (currentTile->state != 1) {
				tileUnchecked.push_back(currentTile);
				
			}
		}
	}

	int check = Vector2Equals(Vector2Zero(), Vector2Zero());
	check = Vector2Equals(Vector2Zero(), Vector2{1,0});
	check = Vector2Equals(Vector2Zero(), Vector2{ 1,1 });
	check = Vector2Equals(Vector2Zero(), Vector2{ 0,1 });

	int currentGroup = 0;
	std::vector<Tile*> tileToCheck;
	tileToCheck.push_back(tileUnchecked.back());
	while (!tileUnchecked.empty()) {
		//get a tile that is unchecked
		tileToCheck.push_back(tileUnchecked.back());
		tileUnchecked.pop_back();

		std::vector<Tile*>tileGroup;
		std::vector<Tile*>sideTileGroup;
		while (!tileToCheck.empty()) {
			//get a tile to check
			Tile* currentTile = tileToCheck.back();
			tileToCheck.pop_back();
			currentTile->debugColor = colorDebugGroup[currentGroup];
			currentTile->state = 2 + currentGroup;
			tileGroup.push_back(currentTile);
			if (Vector2Equals(currentTile->side, Vector2Zero()) != 1) {
				sideTileGroup.push_back(currentTile);
			}

			//Get all tile next to the tile currently being checked
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					Vector2 tPos = { Clamp(currentTile->localX + i,0,nodeGridSize - 1),Clamp(currentTile->localY + j,0,nodeGridSize - 1) };
					Tile* newTile = tileGrid[int(tPos.x)][int(tPos.y)];
					//Add the tile to be checked if in unchecked list 
					if (std::find(tileUnchecked.begin(), tileUnchecked.end(), newTile) != tileUnchecked.end()) {
						//Add the tile to check and remove from unchecked
						tileToCheck.push_back(newTile);
						tileUnchecked.erase(std::remove(tileUnchecked.begin(), tileUnchecked.end(), newTile), tileUnchecked.end());
					}
				}
			}
		}

		//No Side Detected
		if (sideTileGroup.size() == 0) {
			for (Tile* t : tileGroup) {
				t->debugColor = DARKGRAY;
				t->state = 0;
			}
		}

		

		tileGroup.clear();
		sideTileGroup.clear();

		currentGroup++;
		currentGroup = Clamp(currentGroup,0, 4);
	}
}
