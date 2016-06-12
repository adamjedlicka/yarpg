#include "entity.h"
#include "map.h"

Map::Map(const std::string &str) {
	offX = 0;
	offY = 0;
	name = str;

	entities = new Entity *[ENTITY_MAX];
	entitiesCnt = 0;
}

Map::~Map() {}

bool Map::LoadFromFile(const std::string &path) {
	height = 20;
	width = 40;
	spawnX = 20;
	spawnY = 10;

	structures = new Structure **[height];
	for (int i = 0; i < height; ++i)
		structures[i] = new Structure *[width];

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; j++) {
			if ((i == 0 || j == 0 || i == height - 1 || j == width - 1) && structures[i][j] == NULL)
				structures[i][j] = new Wall(this, j, i);
		}
	}

	SpawnEntity(new Skeleton(5, 10));

	return true;
}

void Map::SpawnPlayer(Entity *e) {
	player = e;
	player->SetLevel(this);
	player->SetPos(spawnX, spawnY);
	entities[entitiesCnt++] = player;
}

void Map::SpawnEntity(Entity *e) {
	std::pair< int, int > pos = e->GetPos();
	if ((pos.first < 0 || pos.second < 0 || pos.first >= width || pos.second >= height) && (entitiesCnt < ENTITY_MAX)) {
		delete e;
		return;
	}

	e->SetLevel(this);
	entities[entitiesCnt++] = e;
}

void Map::Tick(Engine *engine) {
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; j++)
			if (structures[i][j] != NULL)
				structures[i][j]->Tick(engine);

	for (int i = 0; i < entitiesCnt; ++i) {
		entities[i]->Tick(engine);
		std::pair< int, int > pos = entities[i]->GetPos();
		if (!entities[i]->IsPlayer() && (pos.first < 0 || pos.second < 0 || pos.first >= width || pos.second >= height))
			entities[i]->Destroy(); // out of the map*/

		if (!entities[i]->Destroyed() && structures[pos.second][pos.first] != NULL) {
			structures[pos.second][pos.first]->Colide(entities[i]);
			entities[i]->Colide(structures[pos.second][pos.first]);
		}
	}

	int colisions = 0;
	for (int i = 0; i < entitiesCnt; ++i) {
		for (int j = 0; j < entitiesCnt; ++j) {
			if (i != j) {
				std::pair< int, int > posA = entities[i]->GetPos();
				std::pair< int, int > posB = entities[j]->GetPos();
				if (posA == posB)
					entities[i]->Colide(entities[j]), ++colisions;
			}
		}
	}
	engine->log << "colisions: " << colisions << std::endl;

	offX = (engine->GetCurBuffer()->GetWidth() / 2) - player->GetPos().first;
	offY = (engine->GetCurBuffer()->GetHeight() / 2) - player->GetPos().second;

	for (int i = 0; i < entitiesCnt; ++i) {
		if (entities[i]->Destroyed()) {
			delete entities[i];
			entities[i] = NULL;
		}
	}
	Entity **entitiesNEW = new Entity *[ENTITY_MAX];
	int tmp = 0;
	for (int i = 0; i < entitiesCnt; ++i) {
		if (entities[i] != NULL) {
			entitiesNEW[tmp++] = entities[i];
		}
	}
	entitiesCnt = tmp;
	entities = entitiesNEW;

	engine->log << "mapName: " << name << ", offsets: " << offX << ", " << offX << std::endl;
	engine->log << "entityCnt: " << entitiesCnt << std::endl;
}

void Map::Render(Buffer *buffer) const {
	buffer->ClearCanvas();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; j++)
			if (structures[i][j] != NULL)
				structures[i][j]->Render(buffer);

	for (int i = 1; i < entitiesCnt; ++i)
		entities[i]->Render(buffer);

	if (entities[0])
		entities[0]->Render(buffer);
}
