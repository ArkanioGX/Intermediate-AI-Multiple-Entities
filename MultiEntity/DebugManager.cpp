#include "DebugManager.h"

void DebugManager::draw()
{
	for (RectangleDebug rd : rectList) {
		if (rd.fill) {
			DrawRectangle(rd.pos.x, rd.pos.y, rd.size.x, rd.size.y, rd.col);
		}
		else {
			DrawRectangleLines(rd.pos.x, rd.pos.y, rd.size.x, rd.size.y, rd.col);
		}
	}
	rectList.clear();
	for (LineDebug ld : lineList) {
		DrawLine(ld.pos.x, ld.pos.y, ld.pos2.x, ld.pos2.y, ld.col);
	}
	lineList.clear();
}

void DebugManager::addRectangle(Vector2 pos, Vector2 size, Color col, bool isFilled)
{
#if _DEBUG
	RectangleDebug rd;
	rd.pos = pos;
	rd.size = size;
	rd.col = col;
	rd.fill = isFilled;
	rectList.push_back(rd);
#endif
}

void DebugManager::addLine(Vector2 pos, Vector2 pos2, Color col)
{
#if _DEBUG
	LineDebug rd;
	rd.pos = pos;
	rd.pos2 = pos2;
	rd.col = col;
	lineList.push_back(rd);
#endif
}
