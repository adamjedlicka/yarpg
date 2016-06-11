#include "player.h"

Player::Player(const std::string &name) : Entity() {
	this->name = name;

	// set stats
	speed = 10;

	// set state
	ticksSinceLastStep = 0;
	destroyed = false;
}

Player::~Player() {}

void Player::Tick(Engine *engine) {
	if (engine->GetKey('d'))
		this->dirX = MIN(this->dirX + 1, 1);

	if (engine->GetKey('a'))
		this->dirX = MAX(this->dirX - 1, -1);

	if (engine->GetKey('s'))
		this->dirY = MIN(this->dirY + 1, 1);

	if (engine->GetKey('w'))
		this->dirY = MAX(this->dirY - 1, -1);

	if (this->ticksSinceLastStep >= this->speed) {
		this->posX += this->dirX;
		this->posY += this->dirY;
		this->ticksSinceLastStep = 0;
	}

	if (engine->GetKey(KEY_UP)) {
		Fireball *f = new Fireball(this->posX, this->posY - 1, 0, -1);
		engine->GetCurLevel()->SpawnEntity(f);
	}

	if (engine->GetKey(KEY_LEFT)) {
		Fireball *f = new Fireball(this->posX - 1, this->posY, -1, 0);
		engine->GetCurLevel()->SpawnEntity(f);
	}

	if (engine->GetKey(KEY_RIGHT)) {
		Fireball *f = new Fireball(this->posX + 1, this->posY, +1, 0);
		engine->GetCurLevel()->SpawnEntity(f);
	}

	if (engine->GetKey(KEY_DOWN)) {
		Fireball *f = new Fireball(this->posX, this->posY + 1, 0, +1);
		engine->GetCurLevel()->SpawnEntity(f);
	}

	this->ticksSinceLastStep++;

	engine->log << "playerPos: " << this->posX << ", " << this->posY << std::endl;
	engine->log << "playerDir: " << this->dirX << ", " << this->dirY << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), 'X', COLOR_BLUE);
}
