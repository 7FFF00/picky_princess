// #include <cstddef> // access to size_t
// #include "Rng.hpp"
#include <cmath> // access to absolute function
#include <iostream> // debug access to std::cout
#include <vector>
#include "Matrix.hpp" // need to load before map, as map calls for a matrix
#include "Rectangle.hpp" // needed for zone mapping for both generation and location zoning
#include "Map.hpp"
#include "Entity.hpp"
#include "Stats.hpp"
#include "Engine.hpp"
#include "BearLibTerminal.h"

//
// MAP CLASS DECLARATION AND USE FUNCTIONS
//

Map::Map(unsigned int width, unsigned int height):
	width(width), height(height) {
	init();
}

Map::~Map() {
}

void Map::init() {
	cell_map = Matrix<Cell*>(width, height, true);
	fov_map = Matrix<bool>(width, height);

	Rectangle test_map_rect = Rectangle(width, height);
	// genDungeonBSP(test_map_rect);
	genRandRooms(30);
	genConnections();
}

int* Map::findWalkable() {
	int* walkable = new int[2];
	int cx, cy;
	for (int i = 0; i < 100; i++) {
		cx = engine.rng(0, width - 1);
		cy = engine.rng(0, height - 1);
		if (canMove(cx, cy)) {
			walkable[0] = cx;
			walkable[1] = cy;
			break;
		}
	}
	if (canMove(walkable[0], walkable[1])) {
		return walkable;
	}
	else {
		for (int x = 0; x < (int)width; x++) {
			for (int y = 0; y < (int)height; y++) {
				if (canMove(x, y)) {
					walkable[0] = x;
					walkable[1] = y;
					break;
				}
			}
		}
	}
	return walkable;
}

bool Map::isInRange(int x, int y) {
	if ((x >= 0 && x < (int)width) && (y >= 0 && y < (int)height)) { return true; }
	else { return false; }
}

bool Map::canMove(int x, int y) {
	if (isInRange(x, y) && isWall(x, y)) {
		// if coord in map AND is wall, return false
		return false;
	}
	else {
		// if coord isnt WALL, test if actor present and IS BLOCKING, else return true
		// std::cout << "testing if can move" << std::endl;
		if ((int)engine.actors.size() > 1) {
			for (int i = 1; i < (int)engine.actors.size(); i++) {
				if (engine.actors[i]->blocks &&
					engine.actors[i]->x == x &&
					engine.actors[i]->y == y) {
					// if actor in list is blocking and exists at coord, return false
					return false;
				}
			}
		}
		// if NOT wall, and NOT blocking actor
		return true;
	}
}

bool Map::isWall(int x, int y) {
	if (cell_map(x, y)->terrain == Terrain::WALL) { return true; }
	else { return false; }
}

float Map::getDistance(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return sqrtf((dx * dx) + (dy * dy));
}

//
// FOV FUNCTIONS
//

bool Map::isInFov(int x, int y) {
	return fov_map(x, y);
}

bool Map::isExplored(int x, int y) {
	return cell_map(x, y)->explored;
}

bool Map::isOpaque(int x, int y) {
	if (isInRange(x, y) &&
		(cell_map(x, y)->terrain == Terrain::WALL ||
		cell_map(x, y)->terrain == Terrain::TREE)) {
		return true;
	} else {
		return false;
	}
}

void Map::computeFov() {
	// run compute fov during each position change for player, ie in player's update()
	if(engine.actors[0]->stats && engine.actors[0]->stats->sight_radius > 0) {
		float x, y, ox, oy;
		// reset fov map to be re-evaluated each time
		for (int x = 0; x < (int)width; x++) {
			for (int y = 0; y < (int)height; y++) {
				fov_map(x, y) = false;
			}
		}

		// cycling through 360 degrees for los tests
		for (int i = 0; i < 360; i++) {
			x = cos((float)i * 0.01745f);
			y = sin((float)i * 0.01745f);
			ox = (float)engine.actors[0]->x + 0.5f;
			oy = (float)engine.actors[0]->y + 0.5f;
			for (int j = 0; j < engine.actors[0]->stats->sight_radius; j++) {
				if ((int)ox >= 0 && (int)ox < (int)engine.map->width &&
					(int) oy >= 0 && (int)oy < (int)engine.map->height) {
						fov_map((int)ox, (int)oy) = true;
						cell_map((int)ox, (int)oy)->explored = true; // setting explored for cells
				}
				if (isOpaque((int)ox, (int)oy)) { j = engine.actors[0]->stats->sight_radius; }
				else {
					ox += x;
					oy += y;
				}
			}
		}
	}
	else {
		for (int x = 0; x < (int)width; x++) {
			for (int y = 0; y < (int)height; y++) {
				fov_map(x, y) = true;
			}
		}
	}
}

void Map::render() {
	static const char tile_wall = '#';
	static const char tile_floor = '.';
	static const char tile_nature = '+';
	static const char tile_water = '~';

	// static const char* bgcolor_wall = "black";
	// static const char* bgcolor_floor = "#908095";
	// static const char* bgcolor_nature = "green";
	// static const char* bgcolor_water = "blue";

	static const char* fgcolor_seen = "#b0b0b0";
	static const char* fgcolor_explored = "#b1acc9";
	static const char* fgcolor_hidden = "black";

	static const char* bgcolor_seen = "#621D59";
	static const char* bgcolor_explored = "#b1acd9";
	static const char* bgcolor_hidden = "black";
	// static const char* fgcolor_explored = "#404040";
	// static const char* fgcolor_hidden = "black";

	// print checkered background for map before printing visible and
	// explored map
	terminal_layer(0);
	int step = 2;
	bool color_swap = false;
	for (int i = 0; i < (int)width; i+= (step * 2)) {
		for (int j = 0; j < (int)height; j+= step) {
			for (int k = i; k < (i + (step * 2)); k++) {
				for (int l = j; l < (j + step); l++) {
					if (color_swap) { terminal_bkcolor("#b1cfdf"); } // lighter blue
					else { terminal_bkcolor("#acc1cf"); } // darker blue
					if (isInRange(k, l)) { terminal_put(k, l, ' '); }
				}
			}
			color_swap = !color_swap;
		}
		color_swap = !color_swap;
	}

	// map rendering
	terminal_layer(0); // map has to be rendered on layer 0 for bg manipulation
	for (int y = 0; y < (int)height; y++) {
		for (int x = 0; x < (int)width; x++) {
			if (isInFov(x, y)) {
				// tile IS in fov
				switch (cell_map(x, y)->terrain) {
					case Terrain::WALL:
						terminal_bkcolor("#606090");
						terminal_color("#9090B0");
						terminal_put(x, y, tile_wall);
						break;
					case Terrain::FLOOR:
						terminal_bkcolor("#bfbfca");
						terminal_color("#cfcfda");
						terminal_put(x, y, tile_floor);
						break;
					case Terrain::NATURE:
						terminal_bkcolor(bgcolor_seen);
						terminal_color(fgcolor_seen);
						terminal_put(x, y, tile_nature);
						break;
					case Terrain::WATER:
						terminal_bkcolor(bgcolor_seen);
						terminal_color(fgcolor_seen);
						terminal_put(x, y, tile_water);
						break;
					default: break;
				}
			}
			else if (isExplored(x, y)) {
				// tile NOT in fov, IS explored
				switch (cell_map(x, y)->terrain) {
					case Terrain::WALL:
						terminal_bkcolor("#9292B2");
						terminal_color("#A0A0C0");
						terminal_put(x, y, tile_wall);
						break;
					case Terrain::FLOOR:
						terminal_bkcolor("#d0d0db");
						terminal_color("#dfdfea");
						terminal_put(x, y, tile_floor);
						break;
					case Terrain::NATURE:
						terminal_bkcolor(bgcolor_seen);
						terminal_color(fgcolor_seen);
						terminal_put(x, y, tile_nature);
						break;
					case Terrain::WATER:
						terminal_bkcolor(bgcolor_seen);
						terminal_color(fgcolor_seen);
						terminal_put(x, y, tile_water);
						break;
					default: break;
				}
			}
			// else {
			// 	tile NOT in fov, and NOT explored
			// 	terminal_bkcolor(bgcolor_hidden);
			// 	terminal_color(fgcolor_hidden);
			// 	switch (cell_map(x, y)->terrain) {
			// 		case Terrain::WALL:
			// 			terminal_put(x, y, tile_wall);
			// 			break;
			// 		case Terrain::FLOOR:
			// 			terminal_put(x, y, tile_floor);
			// 			break;
			// 		case Terrain::NATURE:
			// 			terminal_put(x, y, tile_nature);
			// 			break;
			// 		case Terrain::WATER:
			// 			terminal_put(x, y, tile_water);
			// 			break;
			// 		default: break;
			// 	}
			// }
		}
	}
	terminal_color(engine.default_color);
	terminal_bkcolor(engine.default_bgcolor);
}

//
// MAP CLASS GENERATION FUNCTIONS
//

void Map::paint(int x, int y, Terrain brush) {
	// IF in bounds, paint specific coord, with passed brush, ELSE do nothing
	if (isInRange(x, y)) { cell_map(x, y)->terrain = brush; }
	else { engine.debugMsg("outside of map bounds"); } // debug
}

void Map::paintPath(int x1, int y1, int x2, int y2, Terrain brush) {
	// draw L shaped path forcibly connecting two points taking a random direction
	int dx = x2 - x1;
	int dy = y2 - y1;
	Direction dir_x, dir_y;

	if (dx < 0) { dir_x = Direction::LEFT; }
	else if (dx > 0) { dir_x = Direction::RIGHT; }
	else { dir_x = Direction::NONE; }

	if (dy < 0) { dir_y = Direction::UP; }
	else if (dy > 0) { dir_y = Direction::DOWN; }
	else { dir_y = Direction::NONE; }

	// start longest arm first
	if (std::abs(dx) >= std::abs(dy)) {
		// std::cout << " dx > dy " << std::endl;
		paintWalk(x1, y1, dir_x, brush, std::abs(dx));
		paintWalk(x1 + dx, y1, dir_y, brush, std::abs(dy));
	}
	else {
		// std::cout << " dy > dx " << std::endl;
		paintWalk(x1, y1, dir_y, brush, std::abs(dy));
		paintWalk(x1, y1 + dy, dir_x, brush, std::abs(dx));
	}
}

void Map::paintRect(Rectangle rect, Terrain brush, bool fill) {
	// fill in specified rectangle area, with passed brush
	if (fill) {
		// paint and fill in rectangle
		for (unsigned int y = 0; y < rect.height; y++) {
			for (unsigned int x = 0; x < rect.width; x++) {
				// need to offset x and y based on rectangles actual location
				paint(rect.x + x, rect.y + y, brush);
			}
		}
	}
	else {
		// paint box for rectangle, dont touch anything other than the border
		for (unsigned int y = 0; y < rect.height; y++) {
			for (unsigned int x = 0; x < rect.width; x++) {
				// need to offset x and y based on rectangles actual location
				if (x == 0 || y == 0 || x == rect.width - 1 || y == rect.height - 1) {
					// paint only if at the edge cells
					paint(rect.x + x, rect.y + y, brush);
				}
			}
		}
	}
}

void Map::paintWalk(int x, int y, Direction dir, Terrain brush, int step) {
	//	NONE direction = random walk, 1234 U R D L
	// step -1 loop until map edge, step > 0 loop and decrement until step 0
	int dx, dy; // movement change for next recursion of walk
	if (dir == Direction::NONE) { dx = 0; dy = 0; }
	else if (dir == Direction::UP) { dx = 0; dy = -1; }
	else if (dir == Direction::RIGHT) { dx = 1; dy = 0; }
	else if (dir == Direction::DOWN) { dx = 0; dy = 1; }
	else if (dir == Direction::LEFT) { dx = -1; dy = 0; }

	if (step < 0 || step > 0) {
		step--;
		paint(x, y, brush);
		if (isInRange(x + (2 * dx), y + (2 * dy))) {
			paintWalk(x + dx, y + dy, dir, brush, step);
		}
	}
}

void Map::genDungeonBSP(Rectangle rect, int rmin, int rmax, int split_probability) {
	if (rect.width == width && rect.height == height) {
		//	removing edges for closed map if passed rect is map dimensions
		Rectangle edge_clean_map = Rectangle(rect.width - 2, rect.height - 2, 1, 1);
		genDungeonBSP(edge_clean_map);
	}
	//	splittable if: dimension > rmax OR next subdivision dim > rmin * 2 AND rng < chance to split
	else if (((int)rect.width > rmax || (int)rect.height > rmax) ||
		(((int)rect.width > (3 * rmin) || (int)rect.height > (3 * rmin)) &&
		engine.rng(1, 100) < split_probability)) {
		//	test for which direction to split
		// std::cout << "testing split" << std::endl;
		int roll_orientation; // 0 = split horiz ^v, 1 = split vert <>
		if (((double)(int)rect.height / (double)(int)rect.width) > 1.25) { roll_orientation = 0; }
		else if (((double)(int)rect.width / (double)(int)rect.height) > 1.25) { roll_orientation = 1; }
		else { roll_orientation = engine.rng(0, 1); }

		//	use rng to split
		if (roll_orientation == 0) {
			// split horiz, aka up down
			// std::cout << "H";
			int lower_limit, upper_limit;
			if (((int)rect.height / 3) <= rmin) {
				lower_limit = rmin;
				upper_limit = (int)rect.height - rmin;
			}
			else {
				lower_limit = (int)rect.height / 3;
				upper_limit = (int)rect.height - lower_limit;
			}
			// std::cout << lower_limit << " " << upper_limit << std::endl;
			int top_height = engine.rng(lower_limit, upper_limit);
			int bot_height = (int)rect.height - top_height;
			Rectangle sub_rect_top = Rectangle((int)rect.width, top_height, (int)rect.x, (int)rect.y);
			genDungeonBSP(sub_rect_top, rmin, rmax);
			Rectangle sub_rect_bot = Rectangle((int)rect.width, bot_height,
				(int)rect.x, ((int)rect.y + top_height));
			genDungeonBSP(sub_rect_bot, rmin, rmax);

			//	connect and paint a path between the two zones
			int x1, y1, x2, y2;
			x1 = sub_rect_top.x + (sub_rect_top.width / 2);
			y1 = sub_rect_top.y + (sub_rect_top.height / 2);
			x2 = sub_rect_bot.x + (sub_rect_bot.width / 2);
			y2 = sub_rect_bot.y + (sub_rect_bot.height / 2);

			// print halls to map 1/2
			//	if hallway height > map height / 3, fill with genConnections()
			if (y1 < ((int)height / 3)) {
				Rectangle sub_rect_hallway = Rectangle(x2 + 1 - x1, y2 + 1 - y1, x1, y1);
				paintRect(sub_rect_hallway, Terrain::FLOOR);
			}
		}
		else {
			// split vert, aka left and right
			// std::cout << "V";
			int lower_limit, upper_limit;
			if (((int)rect.width / 3) <= rmin) {
				lower_limit = rmin;
				upper_limit = (int)rect.width - rmin;
			}
			else {
				lower_limit = (int)rect.width / 3;
				upper_limit = (int)rect.width - lower_limit;
			}
			// std::cout << lower_limit << " " << upper_limit << std::endl;
			int left_width = engine.rng(lower_limit, upper_limit);
			int right_width = (int)rect.width - left_width;
			Rectangle sub_rect_left = Rectangle(left_width, (int)rect.height, (int)rect.x, (int)rect.y);
			genDungeonBSP(sub_rect_left, rmin, rmax);
			Rectangle sub_rect_right = Rectangle(right_width, (int)rect.height,
				((int)rect.x + left_width), (int)rect.y);
			genDungeonBSP(sub_rect_right, rmin, rmax);

			//	connect and paint a path between the two zones
			int x1, y1, x2, y2;
			x1 = sub_rect_left.x + (sub_rect_left.width / 2);
			y1 = sub_rect_left.y + (sub_rect_left.height / 2);
			x2 = sub_rect_right.x + (sub_rect_right.width / 2);
			y2 = sub_rect_right.y + (sub_rect_right.height / 2);

			// print halls to map 2/2
			//	if hallway width > map width / 3, fill with genConnections()
			if (x1 < ((int)width / 3)) {
				Rectangle sub_rect_hallway = Rectangle(x2 + 1 - x1, y2 + 1 - y1, x1, y1);
				paintRect(sub_rect_hallway, Terrain::FLOOR);
			}
		}
	}
	else {
		// if rect < 2 min + rng area, then create room, factoring buffer, pass to paintrect
		int buff_x, buff_y, buff_w, buff_h, delta_w, delta_h, lower_limit;
		//	variable dimension rooms
		if ((int)rect.width * 3 / 5 <= rmin) { lower_limit = rmin; }
		else { lower_limit = (int)rect.width * 3 / 5; }
		buff_w = engine.rng(lower_limit, (int)rect.width);
		if ((int)rect.height * 2 / 5 <= rmin) { lower_limit = rmin; }
		else { lower_limit = (int)rect.height * 2 / 5; }
		buff_h = engine.rng(lower_limit, (int)rect.height);
		delta_w = (int)rect.width - buff_w;
		delta_h = (int)rect.height - buff_h;
		buff_x = engine.rng((int)rect.x, (int)rect.x + delta_w);
		buff_y = engine.rng((int)rect.y, (int)rect.y + delta_h);

		//	buffered dimension rooms
		// buff_w = (int)rect.width - 2;
		// buff_h = (int)rect.height - 2;
		// buff_x = (int)rect.x + 1;
		// buff_y = (int)rect.y + 1;

		// print rooms to map
		Rectangle buffered_rect = Rectangle(buff_w, buff_h, buff_x, buff_y);
		paintRect(buffered_rect, Terrain::FLOOR);
	}
	genConnections();
}

void Map::genRandRooms(int attempts) {
	int room_w, room_h, x, y;
	Rectangle rect;
	while (attempts > 0) {
		room_w = engine.rng(3, 10);
		room_h = engine.rng(3, 10);
		x = engine.rng(2, width - room_w - 2);
		y = engine.rng(2, height - room_h - 2);
		rect = Rectangle(room_w, room_h, x, y);
		if (cell_map(x, y)->terrain != Terrain::FLOOR &&
			cell_map(x, y + room_h)->terrain != Terrain::FLOOR &&
			cell_map(x + room_w, y)->terrain != Terrain::FLOOR &&
			cell_map(x + room_w, y + room_h)->terrain != Terrain::FLOOR) {
			// if all four corners not overlapping anything, then print
			paintRect(rect);
		}
		attempts--;
		// terminal_clear(); // debug display
		// render(); // debug display
		// terminal_refresh(); // debug display
	}
}


void Map::genConnections() {
	// generate room connections
	Matrix<bool> explorable = Matrix<bool>(width, height); // default instantiated to false
	int start_x(0), start_y(0); // coord to begin breadth first search
	int count_explorable, count_walkable, connect_x, connect_y; //

	// find viable starting coord to repeat search from
	for (int i = 0; i < (int)width; i++) {
		for (int j = 0; j < (int)height; j++) {
			if (cell_map(i, j)->terrain == Terrain::FLOOR) {
				start_x = i;
				start_y = j;
				break; // exit vertical scan loop as soon as first explorable is found
			}
		}
		if (start_x > 0) { break; }
			// exit horizontal scan loops after first explorable
	}

	do {
		explorable = Matrix<bool>(width, height);
		flagExplorable(start_x, start_y, explorable); // update map of connected cells

		// update explorable and walkable cell counts
		count_walkable = 0;
		count_explorable = 0;
		for (int i = 0; i < (int)width; i++) {
			for (int j = 0; j < (int)height; j++) {
				if (cell_map(i, j)->terrain != Terrain::WALL) {
					count_walkable++;
				}
				if (explorable(i, j)) {
					count_explorable++;
				}
			}
		}

		for (int i = 0; i < (int)width; i++) {
			for (int j = 0; j < (int)height; j++) {
				if (cell_map(i, j)->terrain != Terrain::WALL && !explorable(i, j)) {
					// if cell is walkable but not connected, mark to be connected
					if (connect_x > 0 && connect_y > 0) {
						if (engine.rng(1,10) == 1) {
							break;
						}
					}
					connect_x = i;
					connect_y = j;
				}
			}
		}

		// std::cout << "explorable: " << count_explorable << ", walkable: " << count_walkable << std::endl;
		if (count_walkable > count_explorable) {
			// if still more walkable but inaccessible cells
			// find a disconnected cell and connect to nearest walkable cell
			int explored_x = start_x;
			int explored_y = start_y;
			int explored_delta = calcDelta(connect_x, connect_y, explored_x, explored_y);
			for (int i = 0; i < (int)width; i++) {
				for (int j = 0; j < (int)height; j++) {
					if (explorable(i, j) &&
						(calcDelta(connect_x, connect_y, i, j) < explored_delta)) {
						// if cell is marked as connected AND new delta is closer, update x, y, delta
			   			explored_x = i;
						explored_y = j;
						explored_delta = calcDelta(connect_x, connect_y, i, j);
					}
				}
			}
			paintPath(connect_x, connect_y, explored_x, explored_y);
		}
	}
	while (count_walkable > count_explorable);
}

void Map::flagExplorable(int x, int y, Matrix<bool>& explorable, Terrain bad_tile) {
	// breadth first search using recursion to flag adjacent cells as connected in passed bool map
	if (isInRange(x, y) && (cell_map(x, y)->terrain != bad_tile)) { explorable(x, y) = true; }

	// terminal_clear();
	// render();
	// boolMapRender(explorable);
	// terminal_refresh();

	if (isInRange(x + 1, y) &&
		(explorable(x + 1, y) == false) &&
		(cell_map(x + 1, y)->terrain != bad_tile)) {
			flagExplorable(x + 1, y, explorable, bad_tile);
	}

	if (isInRange(x - 1, y) &&
		(explorable(x - 1, y) == false) &&
		(cell_map(x - 1, y)->terrain != bad_tile)) {
			flagExplorable(x - 1, y, explorable, bad_tile);
	}

	if (isInRange(x, y + 1) &&
		(explorable(x, y + 1) == false) &&
		(cell_map(x, y + 1)->terrain != bad_tile)) {
			flagExplorable(x, y + 1, explorable, bad_tile);
	}

	if (isInRange(x, y - 1) &&
		(explorable(x, y - 1) == false) &&
		(cell_map(x, y - 1)->terrain != bad_tile)) {
			flagExplorable(x, y - 1, explorable, bad_tile);
	}
}

void Map::boolMapRender(Matrix<bool> bool_map) {
	// render function for bool map rendering
	for (int y = 0; y < (int)height; y++) {
		for (int x = 0; x < (int)width; x++) {
			if (bool_map(x, y)) {
				// if true, print blue
				terminal_bkcolor("green");
				terminal_color("green");
				terminal_put(x, y, '.');
			}
			else {
				// else false, print red
				// terminal_bkcolor("black");
				// terminal_color("black");
				// terminal_put(x, y, '.');
			}
		}
	}
}

int Map::calcDelta(int x1, int y1, int x2, int y2) {
	return (std::abs(x2 - x1) + std::abs(y2 - y1));
}

// int findReachable(int x, int y, )
// int testExplore(int x, int y, Terrain bad_tile) {
// 	// traverse map via breadth first and mark traversed as explorable, default no-go tile is wall
//
// }
//
// void testExplorableMap() {
// 	// test individual map for expl:non-expl cells
// }
//
// void testExplorableGen() {
// 	// test mapgen and output average ratio of expl. and non-expl cells
// }

void Map::debugRevealMap() {
	for (int y = 0; y < (int)height; y++) {
		for (int x = 0; x < (int)width; x++) {
			cell_map(x, y)->explored = true;
		}
	}
}
