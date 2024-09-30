#include "BoidActor.h"


BoidActor::BoidActor():
	Actor()
{
	boidComp = new BoidComponent(this);
	texComp = new TextureComponent(this,"Images/BoidSprite.png");
}
