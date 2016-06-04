#include "player.h"

Player::Player(const std::string &name) : Entity() { this->name = name; }

Player::~Player() {}

void Player::Tick(Engine *engine) {}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), 'X');
}
