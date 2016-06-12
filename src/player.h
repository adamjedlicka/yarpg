#ifndef __PLAYER_H
#define __PLAYER_H

#include "engine.h"
#include "entity.h"

class Player : public Entity {
  protected:
	std::string name;

	// stats
	int speed;

	// state
	int ticksSinceLastStep;

  public:
	Player(const std::string &);
	virtual ~Player();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual void Colide(Structure *);
	virtual bool IsPlayer() { return true; };
	virtual bool IsSolid() { return true; };
	virtual bool Destroyed() { return false; };
	virtual bool Attack(int dmg) { return (hp -= dmg) <= 0 ? Destroy(), true : false; };
};

#endif
