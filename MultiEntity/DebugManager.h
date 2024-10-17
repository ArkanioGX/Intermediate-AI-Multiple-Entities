#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

struct RectangleDebug {
	Vector2 pos;
	Vector2 size;
	Color col;
};

class DebugManager
{
private:
	std::vector<RectangleDebug> rectList;

public:

	static DebugManager& instance() {
		static DebugManager inst;
		return inst;
	}

	void draw();

	void addRectangle(RectangleDebug rd);

};

