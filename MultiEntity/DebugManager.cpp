#include "DebugManager.h"

void DebugManager::draw()
{
	for (RectangleDebug rd : rectList) {
		DrawRectangle(rd.pos.x, rd.pos.y, rd.size.x, rd.size.y, rd.col);
	}
	rectList.clear();
}

void DebugManager::addRectangle(RectangleDebug rd)
{
	rectList.push_back(rd);
}
