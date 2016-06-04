#ifndef __PLAYER_H
#define __PLAYER_H

#include "engine.h"

class Player : public Entity {
  protected:
	std::string name;
	int posY, posX;

	Level *level;

  public:
	Player(const std::string &);
	~Player();

	void Tick(Engine *);
	void Render(Buffer *) const;

	int GetPosY() const { return this->posY; };
	int GetPosX() const { return this->posX; };

	Level *GetLevel() const { return this->level; }
	void SetLevel(Level *level) { this->level = level; }

	void SetXY(int x, int y) {
		this->posX = x;
		this->posY = y;
	}
};

#endif
