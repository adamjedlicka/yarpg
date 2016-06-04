#include "map.h"

Map::Map(const std::string &name) {
	this->name = name;
	this->offX = 0;
	this->offY = 0;
}

Map::~Map() {}

bool Map::LoadFromFile(const std::string &path) {
	this->height = 100;
	this->width = 100;
	this->spawnX = 10;
	this->spawnY = 10;

	this->structures.push_back(new Wall(15, 15, this));
	this->structures.push_back(new Wall(14, 15, this));
	this->structures.push_back(new Wall(13, 15, this));
	this->structures.push_back(new Wall(12, 15, this));
	this->structures.push_back(new Wall(11, 15, this));
	this->structures.push_back(new Wall(10, 15, this));
	this->structures.push_back(new Wall(10, 14, this));
	this->structures.push_back(new Wall(10, 13, this));

	return true;
}

void Map::SpawnPlayer(Entity *player) {
	this->player = player;
	this->player->SetLevel(this);
	this->player->SetXY(this->spawnX, this->spawnY);
}

void Map::Tick(Engine *engine) {
	this->player->Tick(engine);
	
	for (const auto &v : this->structures)
		v->Tick(engine);

	for (const auto &v : this->entities)
		v->Tick(engine);
	
	this->offX = (engine->GetCurBuffer()->GetWidth() / 2) - this->player->GetPosX();
	this->offY = (engine->GetCurBuffer()->GetHeight() / 2) - this->player->GetPosY();
}

void Map::Render(Buffer *buffer) const {
	std::stringstream ss;
	ss << this->name;
	buffer->DrawString(0, 0, ss.str());

	for (const auto &v : this->structures)
		v->Render(buffer);

	for (const auto &v : this->entities)
		v->Render(buffer);

	this->player->Render(buffer);
}
