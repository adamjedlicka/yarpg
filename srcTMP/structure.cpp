#include "structure.h"

Wall::Wall(Level *lvl, int x, int y) : Structure(lvl, x, y) {}

Wall::~Wall() {}

void Wall::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '#');
}

void Wall::Colide(Entity *e) {
	std::pair< int, int > ePos = e->GetPos();
	std::pair< int, int > eDir = e->GetDir();

	e->SetPos(ePos.first - eDir.first, ePos.second - eDir.second);
}
