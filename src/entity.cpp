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
	SetDir(0, 0);
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

// ---------------------- MELEE ----------------------
Melee::Melee(int x, int y) : Entity(x, y) {
	ticks = 1;
	damage = 25;
}
Melee::~Melee() {}
void Melee::Tick(Engine *engine) {
	if (ticks-- == 0)
		Destroy();
}
void Melee::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, '+', COLOR_MAGENTA);
}
void Melee::Colide(Entity *e) {
	if (!e->IsPlayer())
		e->Attack(damage);
}

// ---------------------- ENEMY ----------------------
Enemy::Enemy(int x, int y, int health, int dmg, char c, short col, int movSpeed, int attSpeed) : Entity(x, y) {
	hp = health, damage = dmg;
	ch = c, color = col;
	this->movSpeed = movSpeed, this->attSpeed = attSpeed;
	movTicks = 0, attTicks = 0;
}
Enemy::~Enemy() {}

void Enemy::Tick(Engine *engine) {
	Entity *player = engine->GetCurLevel()->GetPlayer();
	int pPosX = player->GetPos().first;
	int pPosY = player->GetPos().second;

	dirX = MIN(MAX(pPosX - posX, -1), 1);
	dirY = MIN(MAX(pPosY - posY, -1), 1);

	if (movTicks > movSpeed) {
		posX += dirX;
		posY += dirY;
		movTicks = 0;
	}

	movTicks++;
	attTicks++;
}

void Enemy::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, ch, color);
}
void Enemy::Colide(Entity *e) {
	if (attTicks > attSpeed && e->IsPlayer()) {
		e->Attack(damage);
		attTicks = 0;
	}
	if (e->IsSolid())
		SetPos(posX - dirX, posY - dirY);
}
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

// ---------------------- DOOR SWITCH ----------------------
DoorSwitch::DoorSwitch(int x, int y, int xFrom, int xTo, int yFrom, int yTo) : Entity(x, y) {
	this->xFrom = xFrom, this->xTo = xTo;
	this->yFrom = yFrom, this->yTo = yTo;
	switched = false;
	ch = '/';
}
DoorSwitch::~DoorSwitch() {}
void DoorSwitch::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, ch, COLOR_WHITE);
}
void DoorSwitch::Colide(Entity *e) {
	if (e->IsPlayer() && !switched) {
		switched = true;
		ch = '\\';
		for (int i = yFrom; i <= yTo; ++i) {
			for (int j = xFrom; j <= xTo; ++j) {
				level->SetStructure(j, i, NULL);
			}
		}
	}
}
