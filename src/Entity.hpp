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

	int size; // default 3, aka medium
		// flag for if an entity's size, 1-5 = Tiny, Small, Medium, Big, Huge, largely determines
		// basic interactions, ie damage reductions from Chop, grabbability into inventory, and for cooking
	FoodGroup type; // to indicate which food group a particular thing belongs

	Entity* parent; // aka a head/owning entity
	Entity* child; // aka a trailing entity that relies on this entity in its update function

	Entity(const char* name, const char* color, int x, int y, char ch, int size = 3);
	~Entity();
	void update();
	void render() const; // const because function doesn't modify entity
	float getDistance(int cx, int cy);
	bool isInFamily(Entity* a); // traverses parents and children to see if self and a are related
	Entity* findHead(); // return highest parent entity, aka owner
	Entity* findTail(); // return lowest child entity
};

#endif
