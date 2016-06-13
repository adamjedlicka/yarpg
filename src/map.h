#ifndef __MAP_H
#define __MAP_H

#include <sstream>
#include <vector>

#include "engine.h"
#include "structure.h"

#define ENTITY_MAX 1024

class Map : public Level {
  protected:
	std::string name;
	int height, width;
	int spawnX, spawnY;

	Entity *player;
	Entity **entities;
	int entitiesCnt;
	Structure ***structures;

	short gameState;
	bool levelLoaded;

  public:
	Map();
	virtual ~Map();

	virtual void LoadLevel(const std::string &);
	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void SetStructure(int, int, Structure *);

	bool LoadFromFile(const std::string &);
	void SpawnPlayer(Entity *);
	void SpawnEntity(Entity *);

	short GameState() { return gameState; };
	void SetPlayer(Entity *e) { player = e; };
};

#endif
