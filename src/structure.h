#ifndef __STRUCTURE_H
#define __STRUCTURE_H

#include "engine.h"

class Wall : public Structure {
  private:
	int posY, posX;

	Level *level;

  public:
	Wall(int, int, Level *);
	~Wall();

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
