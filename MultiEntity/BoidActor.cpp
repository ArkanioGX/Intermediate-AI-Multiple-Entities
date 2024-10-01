#include "BoidActor.h"
#include "Assets.h"

BoidActor::BoidActor():
	Actor()
{
	boidComp = new BoidComponent(this);
	texComp = new TextureComponent(this,Assets::instance().getTexture("Boid"));
}
