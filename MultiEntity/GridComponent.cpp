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
			
			newNode->owner = this;

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
					newTile->ownerNode = newNode;


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
	for (int i = 0; i < gridSizeX; i++) {
		for (int j = 0; j < gridSizeY; j++) {
			grid[i][j]->updateDijkstra();
		}
	}

	
}

Node* GridComponent::getNodeAt(int x, int y)
{
	return grid[x][y];
}

std::vector<tGroup*> GridComponent::getDijkstraPath(tGroup* begin, tGroup* end)
{
	std::vector<std::vector<dijkstraNode>::iterator> openNodeList;
	std::vector<dijkstraNode> nList;
	
	for (int i = 0; i < tGroupsList.size(); i++) {
		dijkstraNode dNode = dijkstraNode();
		dNode.currentTGroup = tGroupsList[i];
		dNode.id = i;
		nList.push_back(dNode);
		

	}
	for (int i = 0; i < nList.size(); i++) {
		openNodeList.push_back(nList.begin() + i);
	}
	std::vector<dijkstraNode>::iterator dnIt = std::find(nList.begin(), nList.end(), begin);
	(*dnIt).dist = 0;
	while (!openNodeList.empty()) {
		std::vector<std::vector<dijkstraNode>::iterator>::iterator minItTemp = openNodeList.begin() + searchMin(openNodeList);
		std::vector<dijkstraNode>::iterator minIt = (*minItTemp);
		openNodeList.erase(minItTemp);
		
		for (int i = 0; i < (*minIt).currentTGroup->tileGroupNearby.size(); i++) {
			tGroup* tgDistCheck = (*minIt).currentTGroup->tileGroupNearby[i];
			std::vector<dijkstraNode>::iterator dnDist = std::find(nList.begin(), nList.end(), tgDistCheck);
			int distAlt = (*minIt).dist + 1;
			if (distAlt < (*dnDist).dist) {
				(*dnDist).dist = distAlt;
				(*dnDist).previousTGroup = (*minIt).currentTGroup;
			}
		}
	}

	std::vector<tGroup*> dPath;

	std::vector<dijkstraNode>::iterator dnItEnd = std::find(nList.begin(), nList.end(), end);
	tGroup* tgCheck = (*dnItEnd).currentTGroup;
	dPath.push_back(tgCheck);
	while (tgCheck != begin && dPath.size() < maxDPath) {
		tgCheck = (*dnItEnd).previousTGroup;
		dnItEnd = std::find(nList.begin(), nList.end(), tgCheck);
		dPath.push_back(tgCheck);
	}
	return dPath;
}

std::vector<Tile*> GridComponent::getAStarPath(Tile* begin, Tile* end) {
	std::vector<AStarTile> openList = std::vector<AStarTile>();
	std::vector<AStarTile> closedList = std::vector<AStarTile>();
	std::vector<std::vector<AStarTile*>> map(gridSizeX*nodeGridSize,std::vector<AStarTile*>(gridSizeY * nodeGridSize,nullptr));;
	std::vector<tGroup*> dPath = getDijkstraPath(begin->currentGroup, end->currentGroup);
	std::vector<Tile*> aPath = std::vector<Tile*>();
	if (dPath.size() == maxDPath) { return std::vector<Tile*>(); }
	//Node map initialization
	for (tGroup* tg : dPath) {
		for (Tile* t : tg->tiles) {
			AStarTile* at = new AStarTile();
			at->x = t->x;
			at->y = t->y;
			at->g = 0;
			at->h = 0;
			at->f = inf;
			at->parent = nullptr;
			at->current = t;
			at->isWalkable = true;
			map[t->x][t->y] = at;
			

			t->debugColor = GOLD;
		}
	}
	map[begin->x][begin->y]->f = 0;
	AStarTile* beginTile = map[begin->x][begin->y];
	AStarTile* endTile = map[end->x][end->y];

	openList.push_back(*beginTile);
	while (!openList.empty()) {
		int minID = searchMin(openList);
		AStarTile currentTile = openList[minID];
		currentTile.current->debugColor = PURPLE;
		openList.erase(openList.begin() + minID);
		closedList.push_back(currentTile);

		if (hasSamePos(currentTile, *map[end->x][end->y])) {
			AStarTile path = *map[end->x][end->y];
			while (!hasSamePos(path, *map[begin->x][begin->y])) {
				Vector2 p = Vector2{ float(path.x),float(path.y) };
				aPath.push_back(path.current);
				path = *path.parent;
			}
			aPath.push_back(path.current);
			return aPath;
		}
		std::vector<AStarTile> nearbyTile = std::vector<AStarTile>();
		int minx = Clamp(currentTile.x - 1, 0, map.size());
		int maxx = Clamp(currentTile.x + 1, 0, map.size());
		int miny = Clamp(currentTile.y - 1, 0, map[0].size());
		int maxy = Clamp(currentTile.y + 1, 0, map[0].size());
		for (int xp = minx; xp <= maxx; xp++) {
			for (int yp = miny; yp <= maxy; yp++) {
				AStarTile* atg = map[xp][yp];
				if (atg!= nullptr){
					if (ifIsInListViaPos(*atg, closedList) == -1) {
						atg->parent = map[currentTile.x][currentTile.y];
						nearbyTile.push_back(*atg);
					}
				}
			}
		}

		for (AStarTile at : nearbyTile) {
			if (ifIsInListViaPos(at,closedList) != -1) {
				continue;
			}

			at.g = at.g + Vector2Distance(Vector2{ float(at.x),float(at.y) }, Vector2{ float(at.parent->x),float(at.parent->y) });
			at.h = Vector2Distance(Vector2{ float(at.x),float(at.y) }, Vector2{ float(endTile->x),float(endTile->y) });
			at.f = at.h + at.g;

			if (ifIsInListViaPos(at,openList) != -1) {
				if (openList[ifIsInListViaPos(at, openList)].g < at.g) {
					continue;
				}
				openList.erase(openList.begin() + ifIsInListViaPos(at, openList));
			}
			map[at.x][at.y]->g = at.g;
			map[at.x][at.y]->h = at.h;
			map[at.x][at.y]->f = at.f;
			openList.push_back(at);
		}
	}
	return aPath;
}

std::vector<Vector2> GridComponent::getPath(Tile* begin, Tile* end)
{
	Vector2 res = Game::instance().resolution;
	float totalXTileSize = res.x / gridSizeX / nodeGridSize;
	float totalYTileSize = res.y / gridSizeY / nodeGridSize;
	std::vector<Vector2> p;
	std::vector<Tile*> ap = getAStarPath(begin, end);
	for (Tile* t : ap) {
		p.push_back(Vector2{(t->x+0.5f) * totalXTileSize, (t->y + 0.5f) * totalYTileSize});
	}
	return p;
}

bool GridComponent::hasSamePos(AStarTile at1, AStarTile at2) {
	return (at1.x == at2.x && at1.y == at2.y);
}

int GridComponent::ifIsInListViaPos(AStarTile value, std::vector<AStarTile> list) {
	for (int i = 0; i < list.size(); i++) {
		if (value.x == list[i].x && value.y == list[i].y) {
			return i;
		}
	}
	return -1;
}



int GridComponent::searchMin(std::vector<AStarTile> list) {
	int fMin = 0;

	for (int j = 1; j < list.size(); j++) {
		if (list[j].f < list[fMin].f)
		{
			fMin = j;
		}
	}
	return fMin;
}

int GridComponent::searchMin(std::vector<std::vector<dijkstraNode>::iterator> list) {
	int minIt = 0;
	for (int i = 1; i < list.size();  i++) {
		if ((*list[minIt]).dist >(*list[i]).dist) {
			minIt = i;
		}
	}
	return minIt;

}

void GridComponent::addTGroup(tGroup* tg)
{
	tGroupsList.push_back(tg);
}

void GridComponent::removeTGroup(tGroup* tg)
{
	std::vector<tGroup*>::iterator it = std::find(tGroupsList.begin(), tGroupsList.end(), tg);
	while (it != tGroupsList.end()) {
		tGroupsList.erase(it);
		it = std::find(tGroupsList.begin(), tGroupsList.end(), tg);
	}
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
		if (lastNodeSelected != currentNode) {
			lastNodeSelected = currentNode;
			currentNode->debugColor = GREEN;
			nodeSelected.push_back(currentNode);
		}

	}
	
	if (IsKeyPressed(KEY_A)) {
		beginPath = currentTile;
		currentTile->debugColor = GREEN;
	}
	if (IsKeyPressed(KEY_Z)) {
		endPath = currentTile;
		currentTile->debugColor = DARKGREEN;
	}

	if (IsMouseButtonReleased(0) || IsMouseButtonReleased(1)) {
		while (!nodeSelected.empty()) {
			nodeSelected.back()->debugColor = RED;
			nodeSelected.back()->updateDijkstra();
			nodeSelected.pop_back();
		}
		lastNodeSelected = nullptr;

		
	}
	if (IsMouseButtonPressed(2)) {
		/*
		std::vector<tGroup*> dp = getDijkstraPath(tGroupsList[0], tGroupsList[tGroupsList.size() - 1]);
		for (int i = 0; i < dp.size(); i++) {
			for (int j = 0; j < dp[i]->tiles.size(); j++) {
				dp[i]->tiles[j]->debugColor = GOLD;
			}
		}*/

		std::vector<Vector2> dp = getPath(beginPath,endPath);
		for (int i = 0; i < dp.size(); i++) {
			float xOffset = totalXTileSize / 2;
			float yOffset = totalYTileSize / 2;
			if (i < dp.size() - 1) {
				DebugManager::instance().addLine(Vector2{ float(dp[i].x),float(dp[i].y) }, Vector2{ float(dp[i + 1].x),float(dp[i + 1].y) });
			}
			
		}
	}
	if (IsMouseButtonReleased(2)) {
		for (int i = 0; i < gridSizeX; i++) {
			for (int j = 0; j < gridSizeY; j++) {
				for (int k = 0; k < nodeGridSize; k++) {
					for (int l = 0; l < nodeGridSize; l++) {
						grid[i][j]->getTileAt(k, l)->debugColor = RED;
					}
				}
			}
		}
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

	while (!tGroups.empty()) {
		tGroup* currentTGroup = tGroups.back();
		//Removes all mentions of the current tGroup
		for (tGroup* tg : currentTGroup->tileGroupNearby) {
			while (std::find(tg->tileGroupNearby.begin(), tg->tileGroupNearby.end(), currentTGroup) != tg->tileGroupNearby.end()) {
				tg->tileGroupNearby.erase(std::remove(tg->tileGroupNearby.begin(), tg->tileGroupNearby.end(), currentTGroup), tg->tileGroupNearby.end());
			}
		}
		
		owner->removeTGroup(currentTGroup);
		delete currentTGroup;
		tGroups.pop_back();
	}


	int currentGroup = 0;
	std::vector<Tile*> tileToCheck;
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
			//currentTile->debugColor = colorDebugGroup[currentGroup];
			currentTile->state = 2 + currentGroup;
			currentTile->currentGroup = nullptr;

			tileGroup.push_back(currentTile);
			if (currentTile->side.x != 0 || currentTile->side.y != 0)  {
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

		tGroup* currentTGroup = new tGroup();
		for (Tile* t : tileGroup) {

			currentTGroup->tiles.push_back(t);
			t->currentGroup = currentTGroup;

			//No Side Detected
			if (sideTileGroup.size() == 0) {
				t->debugColor = DARKGRAY;
				t->state = 0;
			}
		}

		for (Tile* t : sideTileGroup) {
			if (t->side.x != 0) {
				int nPosX = t->ownerNode->nodeX + t->side.x;
				int nPosY = t->ownerNode->nodeY;
				if (nPosX >= 0 && nPosX < gridSizeX) {
					Node* neighborNode = t->ownerNode->owner->getNodeAt(nPosX, nPosY);

					for (int i = -1; i <= 1; i++) {

						int tPosX = int(nodeGridSize + t->localX + t->side.x) % nodeGridSize;
						int tPosY = t->localY + i;
						if (tPosY >= 0 && tPosY < nodeGridSize) {
							Tile* neighborTile = neighborNode->getTileAt(tPosX, tPosY);
							if (neighborTile->state > 1 && std::find(currentTGroup->tileGroupNearby.begin(), currentTGroup->tileGroupNearby.end(), neighborTile->currentGroup) == currentTGroup->tileGroupNearby.end()) {
								currentTGroup->tileGroupNearby.push_back(neighborTile->currentGroup);
								neighborTile->currentGroup->tileGroupNearby.push_back(currentTGroup);
							}
							//
							//
						}
					}
				}
				if (t->side.y != 0) {
					
					nPosX = t->ownerNode->nodeX + t->side.x;
					nPosY = t->ownerNode->nodeY + t->side.y;
					if (nPosY >= 0 && nPosY < gridSizeY && nPosX >= 0 && nPosX < gridSizeX) {
						Node* neighborNode = t->ownerNode->owner->getNodeAt(nPosX, nPosY);
						int tPosX = int(nodeGridSize + t->localX + t->side.x) % nodeGridSize;
						int tPosY = int(nodeGridSize + t->localY + t->side.y) % nodeGridSize;
						Tile* neighborTile = neighborNode->getTileAt(tPosX, tPosY);
						if (neighborTile->state > 1 && std::find(currentTGroup->tileGroupNearby.begin(), currentTGroup->tileGroupNearby.end(), neighborTile->currentGroup) == currentTGroup->tileGroupNearby.end()) {
							currentTGroup->tileGroupNearby.push_back(neighborTile->currentGroup);
							neighborTile->currentGroup->tileGroupNearby.push_back(currentTGroup);
						}

					}
				}
			}
			else if (t->side.y != 0)
			{
				int nPosX = t->ownerNode->nodeX;
				int nPosY = t->ownerNode->nodeY + t->side.y;
				if (nPosY >= 0 && nPosY < gridSizeY) {
					Node* neighborNode = t->ownerNode->owner->getNodeAt(nPosX, nPosY);

					for (int i = -1; i <= 1; i++) {

						int tPosX = t->localX + i;
						int tPosY = int(nodeGridSize + t->localY + t->side.y) % nodeGridSize;
						if (tPosX >= 0 && tPosX < nodeGridSize) {
							Tile* neighborTile = neighborNode->getTileAt(tPosX, tPosY);
							if (neighborTile->state > 1 && std::find(currentTGroup->tileGroupNearby.begin(), currentTGroup->tileGroupNearby.end(), neighborTile->currentGroup) == currentTGroup->tileGroupNearby.end()) {
								currentTGroup->tileGroupNearby.push_back(neighborTile->currentGroup);
								neighborTile->currentGroup->tileGroupNearby.push_back(currentTGroup);
							}
						}
					}
				}
			}
		}
		tGroups.push_back(currentTGroup);
		owner->addTGroup(currentTGroup);

		tileGroup.clear();
		sideTileGroup.clear();

		currentGroup++;
		currentGroup = Clamp(currentGroup,0, 4);
	}
}
