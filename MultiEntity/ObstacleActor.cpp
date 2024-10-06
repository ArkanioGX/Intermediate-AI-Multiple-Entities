#include "ObstacleActor.h"
#include "Assets.h"
#include "Game.h"

ObstacleActor::ObstacleActor()
{
	oc = new ObstacleComponent(this);
	tc = new TextureComponent(this, Assets::instance().getTexture("Obstacle"));

	Game::instance().addObstacle(this);
}
