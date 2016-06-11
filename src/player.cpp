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
	if (engine->GetKey(KEY_RIGHT) && this->ticksSinceLastStep >= this->speed) {
		this->posX++;
		this->ticksSinceLastStep = 0;
	}
	if (engine->GetKey(KEY_LEFT) && this->ticksSinceLastStep >= this->speed) {
		this->posX--;
		this->ticksSinceLastStep = 0;
	}
	if (engine->GetKey(KEY_DOWN) && this->ticksSinceLastStep >= this->speed) {
		this->posY++;
		this->ticksSinceLastStep = 0;
	}
	if (engine->GetKey(KEY_UP) && this->ticksSinceLastStep >= this->speed) {
		this->posY--;
		this->ticksSinceLastStep = 0;
	}
	this->ticksSinceLastStep++;

	engine->log << "playerCoords: " << this->posX << ", " << this->posY << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), 'X', COLOR_RED);
}
