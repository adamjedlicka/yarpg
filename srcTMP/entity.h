#ifndef __ENTITY_H
#define __ENTITY_H

#include "engine.h"

class Fireball : public Entity {
  public:
	Fireball(int, int, int, int);
	virtual ~Fireball();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual void Colide(Structure *);
	virtual void OnDestroy();
};

class FireballExplosion : public Entity {
  public:
	FireballExplosion(int, int);
	virtual ~FireballExplosion();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
};

#endif
