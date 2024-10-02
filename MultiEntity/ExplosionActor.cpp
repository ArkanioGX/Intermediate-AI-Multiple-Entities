#include "ExplosionActor.h"
#include "Assets.h"

ExplosionActor::ExplosionActor(Vector2 posT)
{
	tc = new TextureComponent(this, Assets::instance().getTexture("Explosion"));
	ec = new ExplosionComponent(this);
	pos = posT;
	scale = Vector2{1.5,1.5 };
}
