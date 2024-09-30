#include "TextureComponent.h"
#include "Actor.h"

TextureComponent::TextureComponent(Actor* owner, const char* imgPath) :
	RenderComponent(owner)
{
	if (strcmp(imgPath, "") == 0) {
		imgPath = "Images/PlaceholderTexture.png";
	}
	tex = LoadTexture(imgPath);
}

void TextureComponent::draw()
{
	Actor* own = getOwner();
	Vector2 pos = own->getPosition();
	Vector2 scale = own->getScale();

	DrawTexture(tex, pos.x - (tex.width / 2), pos.y - (tex.height / 2), Color{ (unsigned char)((pos.x / 800) * 255), (unsigned char)255,(unsigned char)((pos.y / 800) * 255), (unsigned char)255 });
}

