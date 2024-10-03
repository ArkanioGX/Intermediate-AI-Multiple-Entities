#include "TextureComponent.h"
#include "Actor.h"
#include "raymath.h"
#include "Assets.h"
#include "Game.h"


TextureComponent::TextureComponent(Actor* owner, Texture2D& texT):
	RenderComponent(owner),
	tex(texT)
{
	color = WHITE;
}

TextureComponent::~TextureComponent()
{
	Game::instance().removeRenderComponent(this);
}

void TextureComponent::draw()
{
	Actor* own = getOwner();
	Vector2 pos = own->getPosition();
	Vector2 scale = own->getScale();
	float angle = own->angle;
	Rectangle src = Rectangle{0,0,float(tex.width),float(tex.height)};
	Rectangle dest = Rectangle{ pos.x,pos.y,src.width*scale.x,src.height*scale.y };
	Vector2 origin = Vector2{ float(tex.width * scale.x / 2) ,float(tex.height * scale.y / 2) };

	DrawTexturePro(tex, src, dest, origin, angle, color);
	//DrawTextureEx(tex, Vector2Subtract(pos, Vector2{ float(tex.width*scale.x/2),float(tex.height*scale.y/2) }), angle, scale.x,color);
}

