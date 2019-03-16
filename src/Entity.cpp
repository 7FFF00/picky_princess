#include <vector>							// for vector containers
#include <iostream>						// for debug console output
#include "Entity.hpp"					// entity header
#include "Engine.hpp" 				// for key handling functions
#include "Matrix.hpp" 				// for handling map interaction, move to AI eventually probably
#include "Map.hpp" 						// for handling map interaction, move to AI eventually probably
#include "Ai.hpp" 						// for handling update instructions
#include "Stats.hpp" 					// for handling entity interactions/stat calculations
#include "Inventory.hpp" 			// for handling inventory/item/equip management
#include "Action.hpp"					// all active interactions are handled by this
#include "Passive.hpp"				// passive skills/stat modifications
#include "BearLibTerminal.h"	// for rendering/output and key handling

const int MAX_NAME_LENGTH = 16;
char name [MAX_NAME_LENGTH];

Entity::Entity(const char* name, const char* color, int x, int y, char ch):
	name(name), color(color), x(x), y(y), avatar(ch), sprite(NULL), blocks(true),
	stats(NULL), ai(NULL), title("") {
	map = engine.map;
	stats = new Stats(this);
	// ai = new PlayerAi();
}

Entity::~Entity() {
	if (stats) { delete stats; }
	if (ai) { delete ai; }
}

void Entity::update() {
	if (ai) { ai->update(); }
}

void Entity::render() const {
	terminal_layer(1);
	if (map->isInFov(x, y)) {
		if (sprite == NULL) {
			terminal_color(color);
			terminal_put(x, y, avatar);
		}
		else {
			terminal_printf(x, y, sprite);
		}
	}
	terminal_color(engine.default_color); // resetting terminal color, as bgcolor wasn't modified
}

float Entity::getDistance(int cx, int cy) {
	return map->getDistance(x, y, cx, cy);
}
