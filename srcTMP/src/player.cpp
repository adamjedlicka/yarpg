#include "player.h"

Player::Player(const std::string &str) : Entity(0, 0) {
	name = str;

	// set stats
	speed = 10;

	// set state
	ticksSinceLastStep = 0;
	destroyed = false;
}

Player::~Player() {}

void Player::Tick(Engine *engine) {
	if (engine->GetKey('d'))
		dirX = MIN(dirX + 1, 1);

	if (engine->GetKey('a'))
		dirX = MAX(dirX - 1, -1);

	if (engine->GetKey('s'))
		dirY = MIN(dirY + 1, 1);

	if (engine->GetKey('w'))
		dirY = MAX(dirY - 1, -1);

	if (ticksSinceLastStep >= speed) {
		posX += dirX;
		posY += dirY;
		ticksSinceLastStep = 0;
	}

	if (engine->GetKey(KEY_RIGHT))
		engine->GetCurLevel()->SpawnEntity(new Fireball(posX + 1, posY, 1, 0));
	if (engine->GetKey(KEY_LEFT))
		engine->GetCurLevel()->SpawnEntity(new Fireball(posX - 1, posY, -1, 0));
	if (engine->GetKey(KEY_UP))
		engine->GetCurLevel()->SpawnEntity(new Fireball(posX, posY - 1, 0, -1));
	if (engine->GetKey(KEY_DOWN))
		engine->GetCurLevel()->SpawnEntity(new Fireball(posX, posY + 1, 0, 1));

	ticksSinceLastStep++;

	engine->log << "playerPos: " << posX << ", " << posY << std::endl;
	engine->log << "playerDir: " << dirX << ", " << dirY << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, 'X', COLOR_BLUE);
}

void Player::Colide(Structure *s) {
	if (s->IsSolid()) {
		dirX = 0, dirY = 0;
	}
}

void Player::Colide(Entity *e) {}
