#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Rng {
public:
	unsigned int seed;
	std::mt19937 rng_engine;

	Rng(); // initialize with no seed, use rd()
	Rng(int seed); // initalized with a passed seed
	~Rng();

	// void reseed(unsigned int new_seed);
	int operator()(int min, int max);
};

#endif
