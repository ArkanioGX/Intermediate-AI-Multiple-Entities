#pragma once
#include <map>
#include "raylib.h"
#include "string"

class Assets
{
public:
	static Assets& instance() {
		static Assets inst;
		return inst;
	}

	static bool addTexture(std::string filePath, std::string texName);
	static Texture2D& getTexture(std::string texName);
private:
	static std::map<std::string, Texture2D> textures;
};

