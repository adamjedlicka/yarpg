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
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '@', COLOR_RED, COLOR_YELLOW);
	buffer->DrawChar(posX + level->GetOff().first - dirX, posY + level->GetOff().second - dirY, '+', COLOR_RED);
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
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '@', COLOR_YELLOW, COLOR_RED);
}
void FireballBlast::Colide(Entity *e) { e->Attack(damage); }

// ---------------------- ENEMY ----------------------
Enemy::Enemy(int x, int y, int health, int dmg, char c, short col) : Entity(x, y) {
	hp = health, damage = dmg;
	ch = c, color = col;
}
Enemy::~Enemy() {}
void Enemy::Tick(Engine *engine) {}
void Enemy::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, ch, color);
}
void Enemy::Colide(Entity *e) { e->Attack(damage); }
bool Enemy::Attack(int dmg) { return (hp -= dmg) <= 0 ? Destroy(), true : false; }

// ---------------------- PORTAL ----------------------
Portal::Portal(int x, int y, const std::string &lvl) : Entity(x, y) { nextLevel = lvl; }
Portal::~Portal() {}
void Portal::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, 'O', COLOR_BLUE);
}
void Portal::Colide(Entity *e) {
	if (e->IsPlayer())
		level->LoadLevel(nextLevel);
}
