#ifndef __ENTITY_H
#define __ENTITY_H

#include "engine.h"

class Fireball : public Entity {
  public:
	Fireball(int, int, int, int);
	virtual ~Fireball();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Structure *) { Destroy(); };
	virtual void OnDestroy();
};

class FireballBlast : public Entity {
  protected:
	int ticks;

  public:
	FireballBlast(int, int);
	virtual ~FireballBlast();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
};

#endif
