#ifndef __ENTITY_H
#define __ENTITY_H

#include "engine.h"

class Fireball : public Entity {
  protected:
	Level *level;
	int posX, posY;
	int dirX, dirY;
	bool destroyed;

	int speed, ticksCnt;

  public:
	Fireball(int, int, int, int);
	~Fireball();

	void Tick(Engine *);
	void Render(Buffer *) const;

	std::pair< int, int > GetPos() const { return std::make_pair(this->posX, this->posY); };
	std::pair< int, int > GetDir() const { return std::make_pair(this->dirX, this->dirY); };
	Level *GetLevel() const { return this->level; };
	void SetLevel(Level *level) { this->level = level; };
	void SetXY(int x, int y) { this->posX = x, this->posY = y; };
	void Colide(Entity *) { this->destroyed = true; };
	void Colide(Structure *) { this->destroyed = true; };
	bool Destroyed() const { return this->destroyed; };
};

#endif
