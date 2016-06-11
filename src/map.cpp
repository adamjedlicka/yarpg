#include "map.h"

Map::Map(const std::string &name) {
	this->name = name;
	this->offX = 0;
	this->offY = 0;
}

Map::~Map() {}

bool Map::LoadFromFile(const std::string &path) {
	this->height = 15;
	this->width = 30;
	this->spawnX = 10;
	this->spawnY = 10;

	this->structures = new Structure *[this->height * this->width];
	for (int i = 0; i < this->height; ++i) {
		for (int j = 0; j < this->width; ++j) {
			if (i == 0 || j == 0 || i == this->height - 1 || j == this->width - 1) {
				if (this->structures[i * this->width + j] == NULL)
					this->structures[i * this->width + j] = new Wall(j, i, this);
			}
		}
	}

	return true;
}

void Map::SpawnPlayer(Entity *player) {
	this->player = player;
	this->player->SetLevel(this);
	this->player->SetXY(this->spawnX, this->spawnY);
}

void Map::Tick(Engine *engine) {
	this->player->Tick(engine);
	int x = this->player->GetPos().first;
	int y = this->player->GetPos().second;
	if (this->structures[y * this->width + x] != NULL) {
		this->structures[y * this->width + x]->Colide(this->player);
		this->player->Colide(this->structures[y * this->width + x]);

		engine->log << "player colision: (" << x << ", " << y << ")" << std::endl;
	}

	for (int i = 0; i < this->height * this->width; ++i)
		if (this->structures[i] != NULL)
			this->structures[i]->Tick(engine);

	for (const auto &v : this->entities) {
		v->Tick(engine);
		int x = v->GetPos().first;
		int y = v->GetPos().second;
		if (this->structures[y * this->width + x] != NULL) {
			this->structures[y * this->width + x]->Colide(v);
			v->Colide(this->structures[y * this->width + x]);

			engine->log << "colision: (" << x << ", " << y << ")" << std::endl;
		}
	}

	this->offX = (engine->GetCurBuffer()->GetWidth() / 2) - this->player->GetPos().first;
	this->offY = (engine->GetCurBuffer()->GetHeight() / 2) - this->player->GetPos().second;

	engine->log << "mapName: " << this->name << ", offsets: " << this->offX << ", " << this->offX << std::endl;
}

void Map::Render(Buffer *buffer) const {
	std::stringstream ss;
	ss << this->name;
	buffer->DrawString(0, 0, ss.str());

	for (int i = 0; i < this->height * this->width; ++i)
		if (this->structures[i] != NULL)
			this->structures[i]->Render(buffer);

	for (const auto &v : this->entities)
		v->Render(buffer);

	this->player->Render(buffer);
}
