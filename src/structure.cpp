#include "structure.h"

Wall::Wall(int y, int x, Level *level) {
	this->posY = y;
	this->posX = x;
	this->level = level;
}

Wall::~Wall() {}

void Wall::Tick(Engine *engine) {}

void Wall::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), '#');
}
