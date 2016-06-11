#include "player.h"

Player::Player(const std::string &name) : Entity() {
	this->name = name;

	// set stats
	speed = 10;

	// set state
	ticksSinceLastStep = 0;
}

Player::~Player() {}

void Player::Tick(Engine *engine) {
	this->dirX = 0;
	this->dirY = 0;

	if (engine->GetKey(KEY_RIGHT))
		this->dirX = MIN(this->dirX + 1, 1);

	if (engine->GetKey(KEY_LEFT))
		this->dirX = MAX(this->dirX - 1, -1);

	if (engine->GetKey(KEY_DOWN))
		this->dirY = MIN(this->dirY + 1, 1);

	if (engine->GetKey(KEY_UP))
		this->dirY = MAX(this->dirY - 1, -1);

	if(this->ticksSinceLastStep >= this->speed) {
		this->posX += this->dirX;
		this->posY += this->dirY;
		this->ticksSinceLastStep = 0;
	}

	this->ticksSinceLastStep++;

	engine->log << "playerPos: " << this->posX << ", " << this->posY << std::endl;
	engine->log << "playerDir: " << this->dirX << ", " << this->dirY << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), 'X', COLOR_RED);
}
