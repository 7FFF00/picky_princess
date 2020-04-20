#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Stats.hpp"
#include "Entity.hpp"
#include "Engine.hpp"
#include "Gui.hpp"

Stats::Stats(Entity* owner) : owner(owner) {
	init();
}

Stats::~Stats() {
}

void Stats::init() {
	// health = tough, patience = stamina, cute = charisma
	max_hp = 10; // max health
	hp = 10; // health
	max_patience = 10; // max stamina
	patience = 7; // current stamina
	max_cute = 10; // max cute
	cute = 7; // current level of cute

	sight_radius = 7; // sight radius

	atk = 1; // melee damage
}

void Stats::attack(Entity* target) {
	std::stringstream msg;
	if (target->stats && !target->stats->isDead()) {
		// if target has STATS module, and NOT dead, entity attacks target
		msg << owner->name << " deals " << atk << " to " << target->name << ".";
		engine.gui->message("#404070", msg);
		target->stats->takeDamage(atk);
	}
	else {
		// target is dead
		msg << target->name << " is dead, the attack is ineffective.";
		engine.gui->message("#404070", msg);
	}
}

void Stats::takeDamage(int damage) {
	std::stringstream msg;
	owner->stats->hp -= damage;
	if (owner->stats->hp <= 0) {
		owner->stats->die();
		msg << owner->name << " died.";
		engine.gui->message("#404070", msg);
	}
}

void Stats::die() {
	owner->avatar = '%';
	owner->sprite = NULL;
	owner->color = "red";
	owner->blocks = false;

	if (owner->parent != NULL) {
		owner->parent->child = NULL;
		owner->parent = NULL;
	}

	if (owner->child != NULL) {
		owner->child->ai = NULL;
		owner->child->parent = NULL;
		owner->child = NULL;
	}
}
