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

	std::pair< int, int > GetPos() const { return std::make_pair(this->posX, this->posY); };

	Level *GetLevel() const { return this->level; }
	void SetLevel(Level *level) { this->level = level; }

	void SetXY(int x, int y) {
		this->posX = x;
		this->posY = y;
	}

	void Colide(Entity *);
};

#endif
