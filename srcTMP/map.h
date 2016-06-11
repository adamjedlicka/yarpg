#ifndef __MAP_H
#define __MAP_H

#include <sstream>
#include <vector>

#include "engine.h"
#include "structure.h"

class Map : public Level {
  protected:
	std::string name;
	int height, width;
	int spawnX, spawnY;

	Entity *player;
	std::vector< Entity * > entities;
	std::vector< Structure * > structures;

  public:
	Map(const std::string &);
	virtual ~Map();

	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;

	bool LoadFromFile(const std::string &);
	void SpawnPlayer(Entity *);
	void SpawnEntity(Entity *);
};

#endif
