#include "Rng.hpp"

Rng::Rng() {
	std::random_device rd;
	seed = rd();
	rng_engine.seed(seed);
}

Rng::Rng(int seed): seed(seed) {
	rng_engine.seed(seed);
}

Rng::~Rng() {
}

// void Rng::reseed(unsigned int new_seed) {
// 	rng_engine.seed(new_seed);
// }

int Rng::operator() (int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng_engine);
}

// extern int rng(int min, int max) {
// 	std::uniform_int_distribution<int> dist(min, max);
// 	return dist(rng_engine);
// }
