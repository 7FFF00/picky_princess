#include "Engine.hpp"

Engine engine(110, 45);

int main() {
	while (engine.gameState != Engine::EXIT) {	// main game loop
		engine.update();	// handles engine updating
		engine.render();	// handles engine output to screen
	}
	return 0;
}
