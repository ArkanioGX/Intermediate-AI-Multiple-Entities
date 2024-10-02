#include "BoidActor.h"
#include "Assets.h"

BoidActor::BoidActor():
	Actor()
{
	texComp = new TextureComponent(this, Assets::instance().getTexture("Boid"));
	boidComp = new BoidComponent(this);
	
}
