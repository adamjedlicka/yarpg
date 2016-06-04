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

	return true;
}

void Map::SpawnPlayer(Entity *player) {
	this->player = player;
	this->player->SetLevel(this);
	this->player->SetXY(this->spawnX, this->spawnY);
}

void Map::Tick(Engine *engine) {
	this->offX = (engine->GetCurBuffer()->GetWidth() / 2) - this->player->GetPosX();
	this->offY = (engine->GetCurBuffer()->GetHeight() / 2) - this->player->GetPosY();
}

void Map::Render(Buffer *buffer) const {
	std::stringstream ss;
	ss << this->name;
	buffer->DrawString(0, 0, ss.str());

	this->player->Render(buffer);
}
