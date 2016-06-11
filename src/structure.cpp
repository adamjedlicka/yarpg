#include "structure.h"

Wall::Wall(int x, int y, Level *level) {
	this->posX = x;
	this->posY = y;
	this->level = level;
}

Wall::~Wall() {}

void Wall::Tick(Engine *engine) {}

void Wall::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), '#');
}

void Wall::Colide(Entity *e) {
	std::pair< int, int > ePos = e->GetPos();
	std::pair< int, int > eDir = e->GetDir();

	e->SetXY(ePos.first - eDir.first, ePos.second - eDir.second);
}
