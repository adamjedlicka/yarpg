#include "entity.h"

// ---------------------- FIREBALL ----------------------
Fireball::Fireball(int pX, int pY, int dX, int dY) : Entity(pX, pY) {
	dirX = dX, dirY = dY;
	damage = 15;
}
Fireball::~Fireball() {}
void Fireball::Tick(Engine *engine) {
	posX += dirX;
	posY += dirY;
}
void Fireball::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '@', COLOR_RED);
	buffer->DrawChar(posX + level->GetOff().first - dirX, posY + level->GetOff().second - dirY, '.', COLOR_RED);
}
void Fireball::Colide(Entity *e) {
	e->Attack(damage);
	Destroy();
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
FireballBlast::FireballBlast(int x, int y) : Entity(x, y) {
	ticks = 1;
	damage = 5;
}
FireballBlast::~FireballBlast() {}
void FireballBlast::Tick(Engine *engine) {
	if (ticks-- == 0)
		Destroy();
}
void FireballBlast::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '$', COLOR_YELLOW);
}
void FireballBlast::Colide(Entity *e) { e->Attack(damage); }

// ---------------------- SKELETON ----------------------
Skeleton::Skeleton(int x, int y) : Entity(x, y) { hp = 20, damage = 5; }
Skeleton::~Skeleton() {}
void Skeleton::Tick(Engine *engine) {}
void Skeleton::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, 'M', COLOR_WHITE);
}
void Skeleton::Colide(Entity *e) {}
bool Skeleton::Attack(int dmg) { return (hp -= dmg) <= 0 ? Destroy(), true : false; }
