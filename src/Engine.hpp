#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Rng.hpp"
#include "Rectangle.hpp"
class Map;
class Entity;
class Action;

class Engine {
public:
	// engine variables
	enum GameState {
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT,
		EXIT
	} gameState;

	class ActionSystem* as;
	Rng rng = Rng();
	const int seed = rng(1, 65500);
	// const int seed = rng(1, 65500); // currently using mt(random_device) for rng
	unsigned int window_width, window_height;
	unsigned int ui_height, ui_width, log_width;
	unsigned int turn_count = 0;
	class Rectangle map_frame;
	const char* default_color;
	const char* default_bgcolor;
	class Gui* gui;

	bool sprite_mode = false;
	int key, mouse; // player input
	// forward declared and declared individually, as these classes reference one another
	Map* map;
	Entity* player;
	std::vector<Entity*> actors;	// container for all actors
	std::vector<Action*> actions;	// container for all constructed actions
	// class PerlinNoise* pn;

	Engine(unsigned int width, unsigned int height);
	~Engine();
	// void load();
	void init();
	void update();
	bool isInFrame(int x, int y);
	void render();
	void debugMsg(const char* message = "Debug");
};

extern Engine engine;

#endif
