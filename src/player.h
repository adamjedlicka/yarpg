#ifndef __PLAYER_H
#define __PLAYER_H

#include "engine.h"
#include "entity.h"

class Player : public Entity {
  protected:
	std::string name;
	int posY, posX;
	int dirY, dirX;
	bool destroyed;

	// stats
	int speed;

	// state
	int ticksSinceLastStep;

	Level *level;

  public:
	Player(const std::string &);
	~Player();

	void Tick(Engine *);
	void Render(Buffer *) const;

	std::pair< int, int > GetPos() const { return std::make_pair(this->posX, this->posY); };
	std::pair< int, int > GetDir() const { return std::make_pair(this->dirX, this->dirY); };

	Level *GetLevel() const { return this->level; }
	void SetLevel(Level *level) { this->level = level; }

	void SetXY(int x, int y) {
		this->posX = x;
		this->posY = y;
	}

	void Colide(Entity *){};
	void Colide(Structure *){};

	bool Destroyed() const { return this->destroyed; };
};

#endif
