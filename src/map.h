#ifndef __MAP_H
#define __MAP_H

#include <string>

#include "engine.h"

class Map : public Level {
  private:
	std::string name;

  public:
	Map(std::string);
	~Map();

	void Tick(Engine *);
	void Render(Buffer *) const;
};

#endif
