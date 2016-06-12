#include <fstream>
#include <iostream>
#include <sstream>

#include "engine.h"
#include "map.h"
#include "player.h"

using namespace std;

int main() {
	Engine e;
	e.Start([](map< string, string > flags, Engine &e) {
		Player *player = new Player(flags["playerName"]);
		Map *level = new Map(flags["levelName"]);
		level->LoadFromFile(flags["levelName"]);
		level->SpawnPlayer(player);
		e.LoadLevel(level);
	});

	return 0;
}