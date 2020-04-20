#include <cmath>
#include <iostream>
#include <string>
#include "Engine.hpp"
#include "Matrix.hpp"
#include "Entity.hpp"
#include "Action.hpp"
#include "ActionSystem.hpp"
#include "Ai.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Action.hpp"
#include "Map.hpp"
#include "BearLibTerminal.h"

void Ai::updateChild() {
	if (owner->child) { owner->child->update(); }
}

// Player AI
PlayerAi::PlayerAi(Entity* e) {
	owner = e;
	mode = 0; // assign default player mode to GRAB
}

void PlayerAi::update() {
	// handle input
	if (owner->stats && !owner->stats->isDead()) {
		// if PLAYER is ALIVE, take input
		has_moved = false;
		prev_x = owner->x;
		prev_y = owner->y;
		int dx = 0, dy = 0; // taking placeholder values for change in position to test before acting
		engine.key = terminal_read();
		switch (engine.key) {
			// MOVEMENTS
			case TK_UP : dy = -1; break;
			case TK_DOWN : dy = 1; break;
			case TK_LEFT : dx = -1; break;
			case TK_RIGHT : dx = 1; break;
			// BUMP MODES
			case TK_G : mode = 0; break; // Grab
			case TK_C : mode = 1; break; // Chop
			case TK_S : mode = 2; break; // Stir
			case TK_H : mode = 3; break; // Heat
			case TK_E : mode = 4; break; // Eat
			// case TK_I : mode = 5; break; // Ice
			// SPECIAL KEYS
			case TK_ESCAPE : engine.gameState = Engine::EXIT; terminal_close(); break;
			case TK_SPACE : handleKey(engine.key); break;
			case TK_TAB : handleKey(engine.key); break;
			// DEBUG KEYS
			case TK_F1 : handleKey(engine.key); break; // DEBUG keys
			case TK_F2 : handleKey(engine.key); break; // DEBUG keys
			case TK_F3 : handleKey(engine.key); break; // DEBUG keys
			case TK_F4 : handleKey(engine.key); break; // DEBUG keys
			case TK_F5 : handleKey(engine.key); break; // DEBUG keys
			case TK_F6 : handleKey(engine.key); break; // DEBUG keys
			default : break;
		}

		if (dx != 0 || dy != 0) {
			// if player attempts a move in any direction
			engine.gameState = Engine::NEW_TURN;
			if (moveOrInteract(owner->x + dx, owner->y + dy)) {
				owner->map->computeFov();
			}
		}
	}
	else {
		// PLAYER is DEAD
		engine.key = terminal_read();
	}
	if (has_moved) {
		updateChild();
	}
}

bool PlayerAi::moveOrInteract(int target_x, int target_y) {
	if (engine.map->canMove(target_x, target_y)) {
		// if nothing is there, then move
		owner->x = target_x;
		owner->y = target_y;
		has_moved = true;
		return true;
	}
	else if (!engine.map->isWall(target_x, target_y)) {
		// if can't move, but isn't wall, check actor list
		for (int i = 0; i < (int)engine.actors.size(); i++) {
			if ((engine.actors[i]->x == target_x) &&
				 (engine.actors[i]->y == target_y)) {
				engine.as->update(engine.actions[engine.actors[0]->ai->mode], owner, engine.actors[i]);
					// mode 0 = GRAB, 1 CHOP, 2 STIR, 3 (HEAT variations), 4 (CHILL variations),
					// 5 EAT, 6 SNIFF, 7 TALK, 8 TRANSFORM
				return true;
			}
		}

		return false;
	}
	else {
		// is wall
		return false;
	}
}

void PlayerAi::handleKey(int ascii) {
	// SPECIAL INPUTS (NON-SIMPLE)
	if (ascii == TK_SPACE) {
	}
	else if (ascii == TK_TAB) {
		engine.gui->clear();
	}

	// DEBUG
	else if (ascii == TK_F1) {
		Entity* npc = new Entity("RedXIII", "red", owner->x + 1, owner->y, 'R');
		npc->ai = new ChildAi(npc, owner);
		// owner->child = npc;
		engine.actors.push_back(npc);

		Entity* npc2 = new Entity("Red14", "red", npc->x + 1, npc->y, 'r');
		npc2->ai = new ChildAi(npc2, npc);
		// npc->child = npc2;
		engine.actors.push_back(npc2);

		Entity* npc3 = new Entity("Red15", "red", npc2->x + 1, npc2->y, 'r');
		npc3->ai = new ChildAi(npc3, npc2);
		// npc2->child = npc3;
		engine.actors.push_back(npc3);

		Entity* npc4 = new Entity("Red16", "red", npc3->x + 1, npc3->y, 'r');
		npc4->ai = new ChildAi(npc4, npc3);
		// npc3->child = npc4;
		engine.actors.push_back(npc4);

		std::string list_of_related_entities = "";
		for (int i = 0; i < (int)engine.actors.size(); i++) {
			if (engine.actors[0]->isInFamily(engine.actors[i])) {
				list_of_related_entities += "X";
				std::cout << "Relationship found: " << engine.actors[i]->name << std::endl;
			}
		}
	}
	else if (ascii == TK_F2) {
		engine.map->debugRevealMap();
	}
	else if (ascii == TK_F3) {
		engine.player->stats->hp = engine.player->stats->max_hp;
	}
	else if (ascii == TK_F4) {
		std::string mode_print = "Current Mode is: ";
		mode_print += std::to_string(mode);
		engine.gui->message("#404070", mode_print.c_str());

		std::string mode_size = "Mode Component Count is: ";
		mode_size += std::to_string(engine.actions[mode]->effect.size());
		std::cout << mode_size << std::endl;
		engine.gui->message("#404070", mode_size.c_str());

		if (engine.actions[mode]->effect.find(AC_POWER) != engine.actions[mode]->effect.end()) {
			std::string mode_power = "Mode Power is: ";
			mode_power += engine.actions[mode]->effect[AC_POWER];
			engine.gui->message("#404070", mode_power.c_str());
		}
	}
	else if (ascii == TK_F5) {
		// font character debug display mode
		terminal_layer(2);
		std::string base = "[U+00";
		std::string mod;
		int x = 2;
		int y = 2;
		for (int i = 0; i < 99; i++) {
			if (i < 10) { mod += "0"; }
			mod = base + std::to_string(i) + "]";
			terminal_printf(x + i, y, mod.c_str());
			if (i > 0 && i % 20 == 0) {
				y++;
				x-= 20;
			}
		}
		terminal_printf(2, 7, "\uE201");
		terminal_refresh();
		terminal_read();
	}
}

// ENEMY AI
EnemyAi::EnemyAi(Entity* e)  {
	owner = e;
}

void EnemyAi::update() {
	has_moved = false;
	if (turn_skip) { turn_skip = false; return; }
	if (owner->stats && owner->stats->isDead()) { return; }
	prev_x = owner->x;
	prev_y = owner->y;
	int target_x = engine.player->x;
	int target_y = engine.player->y;
	if (engine.player->stats->isDead()) {
		// IDLE: random walk, player dead anyways
		target_x = engine.rng(1, (int)owner->map->width);
		target_y = engine.rng(1, (int)owner->map->height);
		moveOrInteract(target_x, target_y);
	}
	else if (engine.player->stats && owner->stats &&
		((int)(owner->getDistance(target_x, target_y)) <= owner->stats->sight_radius)) {
		// CHASE: if TARGET is ALIVE, and ENEMY has STATS, AND ENEMY:TARGET distance in SIGHT RANGE
		moveOrInteract(target_x, target_y);
	}
	else {
		// IDLE: random walk
		target_x = engine.rng(1, (int)owner->map->width);
		target_y = engine.rng(1, (int)owner->map->height);
		moveOrInteract(target_x, target_y);
	}
	if (has_moved) {
		updateChild();
	}
}

void EnemyAi::moveOrInteract(int target_x, int target_y) {
	int dx = target_x - owner->x;
	int dy = target_y - owner->y;
	int cx = (dx > 0 ? 1:-1);
	int cy = (dy > 0 ? 1:-1);
	float distance = owner->getDistance(engine.player->x, engine.player->y);
	if (distance >= 2) {
		// Gap in between Entity and Target, Move closer
		dx = (int)(round(dx / distance));
		dy = (int)(round(dy / distance));
		if (owner->map->canMove(owner->x + dx, owner->y + dy)) {
			owner->x += dx;
			owner->y += dy;
			has_moved = true;
		}
		else if (owner->map->canMove(owner->x + cx, owner->y)) {
			owner->x += cx;
			has_moved = true;
		}
		else if (owner->map->canMove(owner->x, owner->y + cy)) {
			owner->y += cy;
			has_moved = true;
		}
	}
	else {
		// Target adjacent, Attack
		owner->stats->attack(engine.player);
	}
}

// CHILD AI, aka FOLLOWER ENTITY
ChildAi::ChildAi(Entity* e, Entity* p)  {
	owner = e;
	owner->parent = p;
	owner->parent->child = e;
}

void ChildAi::update() {
	has_moved = false;
	// if (turn_skip) { turn_skip = false; return; }
	// else { turn_skip = true; }
	if (owner->stats && owner->stats->isDead()) { return; }

	if (owner->getDistance(owner->parent->ai->prev_x, owner->parent->ai->prev_y) == 1) {
		// if parent WAS adjacent, attempt move

		if(owner->map->canMove(owner->parent->ai->prev_x, owner->parent->ai->prev_y)) {
			// tile is movable, moving, set new prev xy
			prev_x = owner->x;
			prev_y = owner->y;


			int dx = owner->parent->ai->prev_x - owner->x;
			int dy = owner->parent->ai->prev_y - owner->y;
			owner->x += dx;
			owner->y += dy;
			has_moved = true;
		}
	}
	else {
	}
	if (has_moved) {
		updateChild();
	}
}
