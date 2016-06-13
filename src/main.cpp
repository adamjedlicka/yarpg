/** 
 * @mainpage
 * @section Description
 * 
 * Simple RPG, top-down & realtime engine, which can be used with custom made maps, items, enemies and quests
 * 
 * Created by Adam Jedlicka, 2016
 */

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
		short type = 0;
		string typeStr = flags["playerType"];
		if (typeStr == "melee")
			type = TYPE_MELEE;
		else if (typeStr == "ranged")
			type = TYPE_RANGED;
		else
			type = TYPE_MELEE;

		Player *player = new Player(flags["playerName"], type);
		Map *level = new Map();
		level->LoadLevel(flags["levelName"]);
		level->SetPlayer(player);
		e.LoadLevel(level);
	});

	return 0;
}
