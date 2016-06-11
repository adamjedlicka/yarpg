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
void Fireball::Colide(Entity *e) { Destroy(); }
void Fireball::Colide(Structure *s) { Destroy(); }
void Fireball::OnDestroy() {
	/*level->SpawnEntity(new FireballExplosion(posX - 1, posY - 1));
	level->SpawnEntity(new FireballExplosion(posX, posY - 1));
	level->SpawnEntity(new FireballExplosion(posX + 1, posY - 1));
	level->SpawnEntity(new FireballExplosion(posX - 1, posY));
	level->SpawnEntity(new FireballExplosion(posX, posY));
	level->SpawnEntity(new FireballExplosion(posX + 1, posY));
	level->SpawnEntity(new FireballExplosion(posX - 1, posY + 1));
	level->SpawnEntity(new FireballExplosion(posX, posY + 1));
	level->SpawnEntity(new FireballExplosion(posX + 1, posY + 1));*/

	level->SpawnEntity(new FireballExplosion(posX + 1, posY - 1));
	level->SpawnEntity(new FireballExplosion(posX + 1, posY));
	level->SpawnEntity(new FireballExplosion(posX + 1, posY + 1));
}

// ---------------------- FIREBALL EXPLOSION ----------------------
FireballExplosion::FireballExplosion(int x, int y) : Entity(x, y) {}
FireballExplosion::~FireballExplosion() {}
void FireballExplosion::Tick(Engine *engine) { Destroy(); }
void FireballExplosion::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '$', COLOR_YELLOW);
}
