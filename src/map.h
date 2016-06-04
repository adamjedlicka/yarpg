#ifndef __MAP_H
#define __MAP_H

#include <vector>
#include <sstream>

#include "engine.h"
#include "structure.h"

class Map : public Level {
  protected:
	std::string name;
	int height, width;
	int offX, offY;
	int spawnX, spawnY;

	Entity *player;
	std::vector< Entity * > entities;
	std::vector< Structure * > structures;

  public:
	Map(const std::string &);
	~Map();

	bool LoadFromFile(const std::string &);
	void SpawnPlayer(Entity *);

	void Tick(Engine *);
	void Render(Buffer *) const;
	int GetOffX() const { return this->offX; };
	int GetOffY() const { return this->offY; };
};

#endif
