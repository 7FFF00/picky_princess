#include <cmath>
#include <iostream>
#include <string>
#include "Engine.hpp"
#include "Matrix.hpp"
#include "Entity.hpp"
#include "Action.hpp"
#include "Ai.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Action.hpp"
#include "Map.hpp"
#include "BearLibTerminal.h"

// Player AI
PlayerAi::PlayerAi(Entity* e) {
	owner = e;
	mode = 0; // assign default player mode to CHOP
}

void PlayerAi::update() {
	// handle input
	if (owner->stats && !owner->stats->isDead()) {
		// if PLAYER is ALIVE, take input
		int dx = 0, dy = 0; // taking placeholder values for change in position to test before acting
		engine.key = terminal_read();
		switch (engine.key) {
			// MOVEMENTS
			case TK_UP : dy = -1; break;
			case TK_DOWN : dy = 1; break;
			case TK_LEFT : dx = -1; break;
			case TK_RIGHT : dx = 1; break;
			// BUMP MODES
			case TK_C : mode = 0; break; // Chop
			case TK_S : mode = 1; break; // Stir
			case TK_H : mode = 2; break; // Heat
			case TK_I : mode = 3; break; // Ice
			case TK_E : mode = 4; break; // Eat
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
}

bool PlayerAi::moveOrInteract(int target_x, int target_y) {
	if (engine.map->canMove(target_x, target_y)) {
		// if nothing is there, then move
		owner->x = target_x;
		owner->y = target_y;
		return true;
	}
	else if (!engine.map->isWall(target_x, target_y)) {
		// if can't move, but isn't wall, check actor list
		for (int i = 0; i < (int)engine.actors.size(); i++) {
			if ((engine.actors[i]->x == target_x) &&
				 (engine.actors[i]->y == target_y)) {
				engine.actions[engine.actors[0]->ai->mode]->update(owner, engine.actors[i]);
					// mode 0 = CHOP,
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
		Entity* enemy = new Entity("RedXIII", "red", owner->x + 1, owner->y, 'R');
		engine.actors.push_back(enemy);
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
	}
}

// ENEMY AI
EnemyAi::EnemyAi(Entity* e)  {
	owner = e;
}

void EnemyAi::update() {
	if (owner->stats && owner->stats->isDead()) { return; }
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
		}
		else if (owner->map->canMove(owner->x + cx, owner->y)) {
			owner->x += cx;
		}
		else if (owner->map->canMove(owner->x, owner->y + cy)) {
			owner->y += cy;
		}
	}
	else {
		// Target adjacent, Attack
		owner->stats->attack(engine.player);
	}
}
