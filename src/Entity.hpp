#ifndef ENTITY_HPP
#define ENTITY_HPP

enum FoodGroup {
	VEG,			// veg and legume/bean
	FRUIT,		// fruits
	GRAIN,		// grains, cereals
	PROTEIN,	// meats, tofu, nuts, seeds, some legume/beans
	DAIRY			// dairy products;
};

class Entity {
public:
	// entity variables
	const char* name; // entity name
	const char* color; // entity representation color
	const char* title; // entity title, primarily for player/outfit setting
	int x, y; // entity location: coordinates
	class Map* map; // entity location: map
	char avatar; // entity representation character
	const char* sprite; // entity representation sprite
	bool blocks; // is entity blocking? can it be passed or walked through/on top of
	class Stats* stats; // entity will have hp/stats for action use calculation (damage deal/etc)
	class Ai* ai; // self updating/control property of entity

	int size; // flag for if an entity's size, 0, 1, 2 = Small/Med/Large, largely determines
		// basic interactions, ie damage reductions from Chop
	FoodGroup type; // to indicate which food group a particular thing belongs

	class Inventory* inventory; // if entity is a container/has an inventory of other entities
	std::vector<class Action> actives; 	// container for skills
	std::vector<class Passive> passives; // container for passives

	Entity(const char* name, const char* color, int x, int y, char ch);
	~Entity();
	void update();
	void render() const; // const because function doesn't modify entity
	float getDistance(int cx, int cy);
};

#endif
