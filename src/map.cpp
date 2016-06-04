#include "map.h"

Map::Map(std::string name) { this->name = name; }

Map::~Map() {}

void Map::Tick(Engine *engine) {}

void Map::Render(Buffer *buffer) const {
	for (uint8_t i = 0; i < this->name.length(); ++i)
		buffer->DrawChar(0, i, this->name[i]);
		
	buffer->DrawChar(0, buffer->GetWidth(), 'X');
}
