// #include <cstddef> // for size_t
// #include "Rng.hpp"
#include <vector> // access for vectors for entity/map lists
#include <string>
#include "Matrix.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "Entity.hpp"
#include "Ai.hpp"
#include "Action.hpp"
#include "ActionSystem.hpp"
#include "Gui.hpp"
#include "Engine.hpp"
#include "BearLibTerminal.h"
// #include "PerlinNoise.hpp"
// #include "Rectangle.hpp"

#include <iostream> // debug for std::cout

Engine::Engine(unsigned int width, unsigned int height):
	window_width(width), window_height(height), ui_height(10), ui_width(30),
	log_width(50) {
	init();
}

Engine::~Engine() {
}

void Engine::init() {
	//	terminal and display variable instantiation
	terminal_open();
	// size = width x height
	terminal_set("window.size = 140x50; "
							 "font: fonts/ProggySquare.ttf, size=16; "
							 "window.title = 'Picky Princess'");
	default_color = "white";
	default_bgcolor = "black";

	//	systems instantiations
	as = new ActionSystem();
	rng = Rng();

	//	demo data instantiation start
	map_frame = Rectangle(window_width, window_height - 4, 1, 1); // frame_w, frame_h, pos_x, pos_y
	gui = new Gui();
	gui->message("#505070", "It has to be declared that only the bubble of the butts can truly master the wimble and the thimble!");

	map = new Map(window_width - ui_width, window_height - ui_height); // init map first so entities can be given a map

	int* start_coord = map->findWalkable();
	terminal_set("0xE000: sprites/gulp_sheet.png, size=16x16, align=center");
	terminal_set("0xE100: resources/princess_crop.png");
	player = new Entity("Apron", "#7FFF00", start_coord[0], start_coord[1], '@');
	player->ai = new PlayerAi(player);
	player->sprite = "\uE000";
	player->title = "Breadsmith";
	actors.push_back(player);

	start_coord = map->findWalkable();
	Entity* enemy = new Entity("Shroomba", "blue", start_coord[0], start_coord[1], 'M');
	enemy->ai = new EnemyAi(enemy);
	actors.push_back(enemy);

	start_coord = map->findWalkable();
	Entity* enemy2 = new Entity("Curdlord", "orange", start_coord[0], start_coord[1], 'C');
	enemy2->ai = new EnemyAi(enemy2);
	actors.push_back(enemy2);

	start_coord = map->findWalkable();
	Entity* enemy3 = new Entity("Garlic Jr.", "#654321", start_coord[0], start_coord[1], 'G');
	enemy3->ai = new EnemyAi(enemy3);
	actors.push_back(enemy3);

	// Action* basic_attack = new Action();
	// actions.push_back(basic_attack);

	// START_SCREEN
	// fov has not been computed yet
	terminal_print(5, 5, "Picky Princess");
	terminal_refresh();
	// debugMsg();
	gameState = STARTUP;
}

void Engine::update() {
	if (gameState == STARTUP) { map->computeFov(); }
	gameState == IDLE;

	turn_count++;
	for (int i = 0; i < (int)actors.size(); i++) {
		if (actors[i]->ai != NULL) {
			// if gameState == NEW_TURN, then do other turns
			actors[i]->update();
		}
	}
}

bool Engine::isInFrame(int x, int y) {
	// frame is now relative to player position
	if (x > player->x - (int)(window_width / 2) + (int)map_frame.x &&
		x < player->x + (int)(window_width / 2) - (int)map_frame.x &&
		y > player->y - (int)(window_height / 2) + (int)map_frame.y &&
		y < player->y + (int)(window_height / 2) - (int)map_frame.y) {
		return true;
	} else {
		return false;
	}
}

void Engine::render() {
	terminal_clear(); // clear screen
	gui->render();
	map->render();
	// render in reverse order so actors[0] aka player, and older actors are rendered last
	for (int i = (int)actors.size() - 1; i >= 0; i--) { actors[i]->render(); }
	terminal_refresh();
}

void Engine::debugMsg(const char* message) {
	std::cout << message << std::endl;
}
