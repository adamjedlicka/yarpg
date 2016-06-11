#ifndef __STRUCTURE_H
#define __STRUCTURE_H

#include "engine.h"

class Wall : public Structure {
  public:
	Wall(Level *, int, int);
	virtual ~Wall();

	virtual void Render(Buffer *) const;
	virtual void Colide(Entity *);
};

#endif
