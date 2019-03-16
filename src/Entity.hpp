#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity {
public:
	// entity variables
	const char* name; // entity name
	const char* color; // entity representation color
	const char* title; // entity title
	int x, y; // entity location: coordinates
	class Map* map; // entity location: map
	char avatar; // entity representation character
	const char* sprite; // entity representation sprite
	bool blocks; // is entity blocking? can it be passed or walked through/on top of
	class Stats* stats; // entity will have hp/stats for action use calculation (damage deal/etc)
	class Ai* ai; // self updating/control property of entity

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
