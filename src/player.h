#ifndef __PLAYER_H
#define __PLAYER_H

#include "engine.h"

class Player : public Entity {
  protected:
	std::string name;

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

	void Colide(Entity *){};
	void Colide(Structure *){};
};

#endif
