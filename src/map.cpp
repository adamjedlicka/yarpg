#include "entity.h"
#include "map.h"

Map::Map() {
	height = 0;
	width = 0;
	offX = 0;
	offY = 0;
	spawnX = 0;
	spawnY = 0;
	name = "level name";

	player = NULL;

	gameState = UNLOAD_STATE;
	levelLoaded = false;

	entities = new Entity *[ENTITY_MAX];
	entitiesCnt = 0;
}

Map::~Map() {
	for (int i = 0; i < entitiesCnt; ++i) {
		delete entities[i];
	}
	delete[] entities;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (structures[i][j] != NULL)
				delete structures[i][j];
		}
		delete[] structures[i];
	}
	delete[] structures;

	for (Quest *q : quests)
		delete q;
}

void Map::LoadLevel(const std::string &str) {
	name = str;
	gameState = UNLOAD_STATE;
}

bool Map::LoadFromFile(const std::string &file) {
	std::string path = GetPath() + "examples/" + file;

	SML global;
	global.ReadFile(path);

	height = global.GetFragment("global").GetValueAsInt("height");
	width = global.GetFragment("global").GetValueAsInt("width");
	spawnX = global.GetFragment("global").GetValueAsInt("spawnX");
	spawnY = global.GetFragment("global").GetValueAsInt("spawnY");

	structures = new Structure **[height];
	for (int i = 0; i < height; ++i) {
		structures[i] = new Structure *[width];
		for (int j = 0; j < width; ++j)
			structures[i][j] = NULL;
	}

	std::string map = global.GetFragment("global").GetValue("map");
	int mapSize = map.size();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; j++) {
			if ((i * width) + j >= mapSize) {
				std::cout << i << " * " << width << " + " << j << " - " << mapSize << std::endl;
			}

			char c = map[(i * width) + j];
			switch (c) {
			case 'X':
				structures[i][j] = new Wall(this, j, i);
				break;
			case '-':
				structures[i][j] = new Door(this, j, i);
				break;
			}
		}
	}

	global.ForEach([this](const std::string &key, const SML_Fragment &fragment) {
		if (fragment.GetValue("type") == "portal") {
			int posX = fragment.GetValueAsInt("posX");
			int posY = fragment.GetValueAsInt("posY");
			std::string lvl = fragment.GetValue("level");

			SpawnEntity(new Portal(posX, posY, lvl));
		} else if (fragment.GetValue("type") == "doorSwitch") {
			int posX = fragment.GetValueAsInt("posX");
			int posY = fragment.GetValueAsInt("posY");
			int xFrom = fragment.GetValueAsInt("xFrom");
			int xTo = fragment.GetValueAsInt("xTo");
			int yFrom = fragment.GetValueAsInt("yFrom");
			int yTo = fragment.GetValueAsInt("yTo");

			SpawnEntity(new DoorSwitch(posX, posY, xFrom, xTo, yFrom, yTo));
		} else if (fragment.GetValue("type") == "enemy") {
			int posX = fragment.GetValueAsInt("posX");
			int posY = fragment.GetValueAsInt("posY");
			std::string eType = fragment.GetValue("eType");
			int hp = fragment.GetValueAsInt("hp");
			int dmg = fragment.GetValueAsInt("damage");
			int ch = fragment.GetValueAsChar("char");
			short color = fragment.GetColor("color");
			int movSpeed = fragment.GetValueAsInt("movSpeed");
			int attSpeed = fragment.GetValueAsInt("attSpeed");

			SpawnEntity(new Enemy(posX, posY, eType, hp, dmg, ch, color, movSpeed, attSpeed));
		} else if (fragment.GetValue("type") == "NPC") {
			int posX = fragment.GetValueAsInt("posX");
			int posY = fragment.GetValueAsInt("posY");
			std::string text = fragment.GetValue("text");
			std::string questID = fragment.GetValue("questID");
			int ch = fragment.GetValueAsChar("char");
			short color = fragment.GetColor("color");

			SpawnEntity(new NPC(posX, posY, ch, color, text, questID));
		} else if (fragment.GetValue("type") == "quest") {
			std::string ID = fragment.GetValue("ID");
			std::string text = fragment.GetValue("text");
			std::string objective = fragment.GetValue("object");
			int times = fragment.GetValueAsInt("times");
			short type = fragment.GetValueAsInt("qType");

			quests.push_back(new Quest(ID, text, objective, type, times));
		} else if (fragment.GetValue("type") == "item") {
			int posX = fragment.GetValueAsInt("posX");
			int posY = fragment.GetValueAsInt("posY");
			int movSpeed = fragment.GetValueAsInt("movSpeed");
			int armor = fragment.GetValueAsInt("armor");
			int damage = fragment.GetValueAsInt("damage");
			char ch = fragment.GetValueAsChar("char");
			short color = fragment.GetColor("color");

			SpawnEntity(new ItemGiver(posX, posY, movSpeed, armor, damage, ch, color));
		}
	});

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
	if (levelLoaded == true && quests.size() == 0) {
		gameState = WIN_STATE;
		return;
	}

	engine->log << "levelName: " << name << ", gameState: " << gameState << std::endl;

	if (levelLoaded == false && gameState == UNLOAD_STATE) {
		LoadFromFile(name);
		SpawnPlayer(player);

		levelLoaded = true;
		gameState = OK_STATE;
	} else if (levelLoaded == true && gameState == UNLOAD_STATE) {
		for (int i = 0; i < entitiesCnt; ++i) {
			if (!entities[i]->IsPlayer())
				delete entities[i];
			entities[i] = NULL;
		}
		entitiesCnt = 0;

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (structures[i][j] != NULL)
					delete structures[i][j];
			}
			delete[] structures[i];
		}
		delete[] structures;

		LoadFromFile(name);
		SpawnPlayer(player);

		levelLoaded = true;
		gameState = OK_STATE;
	}

	if (gameState != OK_STATE) {
		return;
	}

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; j++)
			if (structures[i][j] != NULL)
				structures[i][j]->Tick(engine);

	for (int i = 0; i < entitiesCnt; ++i) {
		entities[i]->Tick(engine);
		std::pair< int, int > pos = entities[i]->GetPos();
		if ((pos.first < 0 || pos.second < 0 || pos.first >= width || pos.second >= height))
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
			if (entities[i]->IsPlayer())
				gameState = LOOSE_STATE;
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
	delete[] entities;
	entities = entitiesNEW;

	engine->log << "mapName: " << name << ", offsets: " << offX << ", " << offX << std::endl;
	engine->log << "entityCnt: " << entitiesCnt << std::endl;
}

void Map::Render(Buffer *buffer) const {
	if (gameState == LOOSE_STATE) {
		std::string text = "You loose! Press enter to continue...";
		buffer->DrawString(buffer->GetWidth() / 2 - text.size() / 2, buffer->GetHeight() / 2, text);

		return;
	}

	if (gameState == WIN_STATE) {
		std::string text = "You win! Press enter to continue...";
		buffer->DrawString(buffer->GetWidth() / 2 - text.size() / 2, buffer->GetHeight() / 2, text);

		return;
	}

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; j++)
			if (structures[i][j] != NULL)
				structures[i][j]->Render(buffer);

	for (int i = 1; i < entitiesCnt; ++i)
		entities[i]->Render(buffer);

	if (entities[0])
		entities[0]->Render(buffer);

	unsigned int questWidth = buffer->GetWidth() / 5;
	buffer->DrawString(buffer->GetWidth() - questWidth, 0, "Quests: ");
	int index = 1;
	for (Quest *q : quests) {
		if (q->Active()) {
			std::string text = q->GetText();
			while (text.size() > questWidth) {
				buffer->DrawString(buffer->GetWidth() - questWidth, 1 + index++, text.substr(0, questWidth));
				text = text.substr(questWidth);
			}
			buffer->DrawString(buffer->GetWidth() - questWidth, 1 + index++, text.substr(0, questWidth));
			index++;
		}
	}
}

void Map::SetStructure(int x, int y, Structure *s) {
	if (structures[y][x] == NULL)
		return;

	delete structures[y][x];
	structures[y][x] = s;
}

void Map::ActivateQuest(const std::string &ID) {
	for (Quest *q : quests) {
		if (q->GetID() == ID)
			q->Activate();
	}
}

void Map::SendQuestEvent(const std::string &obj, short type) {
	for (auto it = quests.begin(); it != quests.end(); it++) {
		Quest *q = *it;
		if (q->GetType() == type && q->GetObjective() == obj) {
			int progress = q->Progress();
			if (progress == 0) {
				it = quests.erase(it);
				delete q;
				if (it == quests.end())
					return;
			}
		}
	}
}

Quest::Quest(const std::string &ID, const std::string &text, const std::string &objective, short type, int times) {
	this->ID = ID, this->text = text, this->objective = objective;
	this->type = type, this->times = times;
	active = false;
}

Quest::~Quest() {}

std::string Quest::GetText() {
	if (times == 1) {
		return text;
	} else {
		std::stringstream ss;
		ss << text << " (" << times << " remaining)";
		return ss.str();
	}
}

int Quest::Progress() {
	if (--times == 0) {
		active = false;
	}
	return times;
}
