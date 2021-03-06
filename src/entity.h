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

class Melee : public Entity {
  protected:
	int damage;
	int ticks;

  public:
	Melee(int, int);
	virtual ~Melee();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
};

class Enemy : public Entity {
  protected:
	std::string enemyType;
	int damage, movSpeed, attSpeed;
	char ch;
	short color;

	int movTicks, attTicks;

  public:
	Enemy(int, int, const std::string &, int, int, char, short, int, int);
	virtual ~Enemy();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual bool Attack(int);
	virtual bool IsSolid() { return true; }
	virtual void OnDestroy();
};

class NPC : public Entity {
  protected:
	char ch;
	short color;
	std::string text;
	std::string questID;
	bool showText;
	int showTextTicks;

  public:
	NPC(int, int, char, short, const std::string &, const std::string &);
	~NPC();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
	virtual bool IsSolid() { return true; };
};

class ItemGiver : public Entity {
  protected:
	int movSpeed, armor, damage;
	char ch;
	short color;

  public:
	ItemGiver(int, int, int, int, int, char, short);
	~ItemGiver();

	void Render(Buffer *) const;
	void Colide(Entity *);
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
