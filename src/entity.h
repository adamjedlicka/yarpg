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

class Enemy : public Entity {
  protected:
	int damage;
	char ch;
	short color;

  public:
	Enemy(int, int, int, int, char, short);
	virtual ~Enemy();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual bool Attack(int);
};

class Portal : public Entity {
  protected:
	std::string nextLevel;

  public:
	Portal(int, int, const std::string &);
	virtual ~Portal();

	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
};

class DoorSwitch : public Entity {
  protected:
	int xFrom, xTo, yFrom, yTo;
	bool switched;
	char ch;

  public:
	DoorSwitch(int, int, int, int, int, int);
	virtual ~DoorSwitch();

	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
};

#endif
