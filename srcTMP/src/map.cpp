#include "map.h"

Map::Map(const std::string &str) {
	offX = 0;
	offY = 0;
	name = str;
}

Map::~Map() {}

bool Map::LoadFromFile(const std::string &path) {
	height = 20;
	width = 40;
	spawnX = 10;
	spawnY = 5;

	structures = new Structure *[height * width];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if ((i == 0 || j == 0 || i == height - 1 || j == width - 1) && structures[i * width + j] == NULL) {
				structures[i * width + j] = new Wall(this, j, i);
			}
		}
	}

	return true;
}

void Map::SpawnPlayer(Entity *e) {
	player = e;
	player->SetLevel(this);
	player->SetPos(spawnX, spawnY);
	entities.push_back(e);
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

void Map::Tick(Engine *engine) {
	for (Entity *e : entities) {
		std::pair< int, int > pos = e->GetPos();
		std::pair< int, int > dir = e->GetDir();
		int posX = pos.first + dir.first;
		int posY = pos.second + dir.second;

		if (posX < 0 || posY < 0 || posX >= width || posY >= height) {
			e->Destroy(); // entity is out of the map
			continue;
		}

		if (structures[posY * width + posX] != NULL) { // there will be a colision next tick
			e->Colide(structures[posY * width + posX]);
		}
	}

	auto it = entities.begin();
	while (it != entities.end()) {
		if ((*it)->Destroyed()) {
			delete *it;
			it = entities.erase(it);
		} else {
			++it;
		}
	}

	for (Entity *e : entities)
		e->Tick(engine);





/*	for (int i = 0; i < height * width; ++i)
		if (structures[i] != NULL)
			structures[i]->Tick(engine);
*/
	for (Entity *e : entities) {
		std::pair< int, int > pos = e->GetPos();

		if (pos.first < 0 || pos.second < 0 || pos.first >= width || pos.second >= height) {
			e->Destroy();
			continue;
		}

		if (structures[pos.second * width + pos.first] != NULL) {
			structures[pos.second * width + pos.first]->Colide(e);
		}
	}

	this->offX = (engine->GetCurBuffer()->GetWidth() / 2) - this->player->GetPos().first;
	this->offY = (engine->GetCurBuffer()->GetHeight() / 2) - this->player->GetPos().second;

	engine->log << "mapName: " << this->name << ", offsets: " << this->offX << ", " << this->offX << std::endl;
	engine->log << "entityCnt: " << this->entities.size() << std::endl;
}

void Map::Render(Buffer *buffer) const {
	for (int i = 0; i < height * width; ++i) {
		if (structures[i] != NULL) {
			structures[i]->Render(buffer);
		}
	}

	for (Entity *e : entities)
		e->Render(buffer);

	if (player != NULL)
		player->Render(buffer);
}
