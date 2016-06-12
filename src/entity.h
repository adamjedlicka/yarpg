#ifndef __ENTITY_H
#define __ENTITY_H

#include "engine.h"

class Fireball : public Entity {
  protected:
	int damage;

  public:
	Fireball(int, int, int, int);
	virtual ~Fireball();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Structure *) { Destroy(); };
	virtual void Colide(Entity *);
	virtual void OnDestroy();
};

class FireballBlast : public Entity {
  protected:
	int ticks;
	int damage;

  public:
	FireballBlast(int, int);
	virtual ~FireballBlast();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
};

class Skeleton : public Entity {
  protected:
	int damage;

  public:
	Skeleton(int, int);
	virtual ~Skeleton();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual bool Attack(int);
};

#endif
