#include "map.h"

Map::Map(const std::string &str) {
	offX = 0;
	offY = 0;
	name = str;
}

Map::~Map() {}

bool Map::LoadFromFile(const std::string &path) {
	height = 50;
	width = 100;
	spawnX = 10;
	spawnY = 10;

	return true;
}

void Map::SpawnPlayer(Entity *e) {
	player = e;
	player->SetLevel(this);
	player->SetPos(spawnX, spawnY);
}

void Map::SpawnEntity(Entity *entity) {
	std::pair< int, int > pos = entity->GetPos();
	if (pos.first < 0 || pos.first >= width || pos.second < 0 || pos.second >= height) {
		delete entity;
		return;
	}

	entity->SetLevel(this);
	entities.push_back(entity);
}

void Map::Tick(Engine *engine) {}

void Map::Render(Buffer *buffer) const {}
