#include "entity.h"

Fireball::Fireball(int posX, int posY, int dirX, int dirY) {
	this->posX = posX;
	this->posY = posY;
	this->dirX = dirX;
	this->dirY = dirY;
	this->destroyed = false;

	this->speed = 5;
	this->ticksCnt = 0;
}

Fireball::~Fireball() {}

void Fireball::Tick(Engine *engine) {
	this->ticksCnt++;
	if (this->ticksCnt >= this->speed) {
		this->posX += this->dirX;
		this->posY += this->dirY;
		this->ticksCnt = 0;
	}
}

void Fireball::Render(Buffer *buffer) const {
	buffer->DrawChar(this->posY + this->level->GetOffY(), this->posX + this->level->GetOffX(), '@', COLOR_RED);
	buffer->DrawChar(this->posY + this->level->GetOffY() - this->dirY, this->posX + this->level->GetOffX() - this->dirX,
					 '.', COLOR_RED);
}
