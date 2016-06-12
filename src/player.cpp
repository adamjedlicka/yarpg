#include "player.h"

Player::Player(const std::string &str) : Entity(0, 0) {
	name = str;

	// set stats
	speed = 10;
	hp = 20;

	// set state
	ticksSinceLastStep = 0;
}

Player::~Player() {}

void Player::Tick(Engine *engine) {
	std::pair< int, int > dir = engine->Direction('w', 'a', 's', 'd');
	dirX += dir.first, dirY += dir.second;
	dirX = MAX(dirX, -1), dirX = MIN(dirX, 1);
	dirY = MAX(dirY, -1), dirY = MIN(dirY, 1);

	if (ticksSinceLastStep >= speed) {
		posX += dirX;
		posY += dirY;
		ticksSinceLastStep = 0;
	}

	dir = engine->Direction(KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT);
	if (dir.first != 0 || dir.second != 0)
		engine->GetCurLevel()->SpawnEntity(new Fireball(posX, posY, dir.first, dir.second));

	ticksSinceLastStep++;

	engine->log << "playerPos: " << posX << ", " << posY << std::endl;
	engine->log << "playerDir: " << dirX << ", " << dirY << std::endl;
	engine->log << "player HP: " << hp << std::endl;
}

void Player::Render(Buffer *buffer) const {
	buffer->DrawChar(posX + level->GetOff().first, posY + level->GetOff().second, 'X', COLOR_BLUE);
}

void Player::Colide(Structure *s) {
	if (s->IsSolid()) {
		dirX = 0, dirY = 0;
	}
}

void Player::Colide(Entity *e) {}
