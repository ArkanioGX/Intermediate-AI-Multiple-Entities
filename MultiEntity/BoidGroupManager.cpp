#include "BoidGroupManager.h"
#include "raymath.h"

 

BoidGroupManager* BoidGroupManager::instance = nullptr;

BoidGroupManager::BoidGroupManager()
{
    GridContent = std::vector<std::vector<std::vector<BoidActor*>>>(int(floor(1920/cellSizeX)), std::vector<std::vector<BoidActor*>>(int(floor(1080 / cellSizeY)), std::vector<BoidActor*>(0)));
}

BoidGroupManager *BoidGroupManager::Instance()
{
    if (instance == nullptr) {
        instance = new BoidGroupManager();
    }
    return instance;
    
}

Vector2 BoidGroupManager::getGridPos(Vector2 worldPos)
{
    worldPos = Vector2Clamp(worldPos, Vector2{ 0,0}, Vector2{ 1919,1079 });
    int xPos = floor(worldPos.x / cellSizeX);
    int yPos = floor(worldPos.y / cellSizeY);

    return Vector2{ float(xPos),float(yPos) };
}

bool BoidGroupManager::AddChild(BoidActor* boid, Vector2 newPos)
{
    //Remove Child From Previous Grid
    Vector2 oldPos = boid->getComponent<BoidComponent*>()->getGridParent();
    if (Vector2Equals(oldPos, Vector2{ -1,-1 }) == 0) {
        while (std::find(GridContent[int(oldPos.x)][int(oldPos.y)].begin(), GridContent[int(oldPos.x)][int(oldPos.y)].end(), boid) != GridContent[int(oldPos.x)][int(oldPos.y)].end()) {
            GridContent[int(oldPos.x)][int(oldPos.y)].erase(std::remove(GridContent[int(oldPos.x)][int(oldPos.y)].begin(), GridContent[int(oldPos.x)][int(oldPos.y)].end(), boid), GridContent[int(oldPos.x)][int(oldPos.y)].end());
        }
    }
    GridContent[int(newPos.x)][int(newPos.y)].push_back(boid);
    return false;
}

std::vector<BoidActor*> BoidGroupManager::getBoids(Vector2 gridPos)
{
    std::vector<BoidActor*> boidsList;
    for (int i = -cellNeighborCheck; i <= cellNeighborCheck ; i++) {
        for (int j = -cellNeighborCheck; j <= cellNeighborCheck ; j++) {
            Vector2 nextGridPos = Vector2{ gridPos.x + i, gridPos.y + j };
            if (nextGridPos.x < minGrid.x) {
                continue;
            }
            if (nextGridPos.x > maxGrid.x) {
                continue;
            }
            if (nextGridPos.y < minGrid.y) {
                continue;
            }
            if (nextGridPos.y > maxGrid.y) {
                continue;
            }

            for (BoidActor* boid : GridContent[int(gridPos.x)][int(gridPos.y)]) {
                boidsList.push_back(boid);
            }
        }
    }
    return boidsList;
}
