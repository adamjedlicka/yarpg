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

	ticksSinceLastStep++;

	engine->log << "playerPos: " << posX << ", " << posY << std::endl;
	engine->log << "playerDir: " << dirX << ", " << dirY << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(posY + level->GetOff().first, posX + level->GetOff().second, 'X', COLOR_BLUE);
}
