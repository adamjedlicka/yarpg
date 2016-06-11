#include "entity.h"

// ---------------------- FIREBALL ----------------------
Fireball::Fireball(int pX, int pY, int dX, int dY) : Entity(pX, pY) { dirX = dX, dirY = dY; }
Fireball::~Fireball() {}
void Fireball::Tick(Engine *engine) {
	posX += dirX;
	posY += dirY;
}
void Fireball::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '@', COLOR_RED);
	buffer->DrawChar(posX + level->GetOff().first - dirX, posY + level->GetOff().second - dirY, '.', COLOR_RED);
}
void Fireball::OnDestroy() {
	level->SpawnEntity(new FireballBlast(posX - 1, posY - 1));
	level->SpawnEntity(new FireballBlast(posX, posY - 1));
	level->SpawnEntity(new FireballBlast(posX + 1, posY - 1));
	level->SpawnEntity(new FireballBlast(posX - 1, posY));
	level->SpawnEntity(new FireballBlast(posX, posY));
	level->SpawnEntity(new FireballBlast(posX + 1, posY));
	level->SpawnEntity(new FireballBlast(posX - 1, posY + 1));
	level->SpawnEntity(new FireballBlast(posX, posY + 1));
	level->SpawnEntity(new FireballBlast(posX + 1, posY + 1));
}

// ---------------------- FIREBALL BLAST ----------------------
FireballBlast::FireballBlast(int x, int y) : Entity(x, y) { ticks = 1; };
FireballBlast::~FireballBlast() {}
void FireballBlast::Tick(Engine *engine) {
	if (ticks-- == 0)
		Destroy();
}
void FireballBlast::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '$', COLOR_YELLOW);
}