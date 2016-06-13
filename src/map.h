#ifndef __MAP_H
#define __MAP_H

#include <sstream>
#include <vector>

#include "engine.h"
#include "structure.h"

#define ENTITY_MAX 1024

class Quest;

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

	std::vector< Quest * > quests;

  public:
	Map();
	virtual ~Map();

	virtual void LoadLevel(const std::string &);
	virtual void Tick(Engine *);
	virtual void Render(Buffer *) const;
	virtual void SetStructure(int, int, Structure *);
	virtual Entity *GetPlayer() { return player; };
	virtual void ActivateQuest(const std::string &);
	virtual void SendQuestEvent(const std::string &, short);

	bool LoadFromFile(const std::string &);
	void SpawnPlayer(Entity *);
	void SpawnEntity(Entity *);

	short GameState() { return gameState; };
	void SetPlayer(Entity *e) { player = e; };
};

class Quest {
  protected:
	std::string ID;
	std::string text;
	std::string objective;
	short type;
	int times;
	bool active;

  public:
	Quest(const std::string &, const std::string &, const std::string &, short, int);
	~Quest();

	bool Active() { return active; };
	void Activate() { active = true; }
	std::string GetID() { return ID; };
	std::string GetText();
	std::string GetObjective() { return objective; };
	short GetType() { return type; };
	int Progress();
};

#endif
