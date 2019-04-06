#ifndef STATS_HPP
#define STATS_HPP

class Entity;

class Stats {
public:
	Entity* owner; // reference to entity that owns stats component
	int level; // level
	int exp; // experience

	int max_hp, hp;	// hp, health points
	int max_patience, patience;	//
	int max_cute, cute;	//

	int sight_radius; // sight radius

	int atk; // strength, melee damage, default is 1 for most things
		// size = damage reduction, 0 is small, 1 is medium, 2 is large

	int sweet; // flavor strength, 1-10
	int salty; // flavor strength, 1-10
	int sour; // flavor strength, 1-10
	int bitter; // flavor strength, 1-10
	int umami; // flavor strength, 1-10
	int spicy; // spiciniess flag, goes from 0-X (probably 3)

	Stats(Entity* owner);
	~Stats();
	void init();

	inline bool isDead() { return hp <= 0; };

	void attack(Entity* target);
	void takeDamage(int damage);
	void die();
};

#endif
