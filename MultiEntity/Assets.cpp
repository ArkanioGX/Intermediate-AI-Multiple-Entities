#include "Assets.h"

std::map<std::string,Texture2D> Assets::textures;

bool Assets::addTexture(std::string filePath, std::string texName)
{
    if (textures[texName].id == 0) {
        textures[texName] = LoadTexture(filePath.c_str());
        return true;
    }
    return false;
}

Texture2D& Assets::getTexture(std::string texName)
{
    return textures[texName];
}
