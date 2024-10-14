#include "GridComponent.h"
#include "raylib.h"
#include "Actor.h"

GridComponent::GridComponent(Actor* owner):
	Component(owner)
{
	for (int i = 0; i < gridSizeX ;i++) {
		for (int j = 0; j < gridSizeY; j++) {
			Node* newNode= new Node();
			newNode->x = i;
			newNode->y = j;
			newNode->state = GetRandomValue(0,1);
			grid[i][j] = newNode;
		}
	}
}

Node* GridComponent::getNodeAt(int x, int y)
{
	return grid[x][y];
}


