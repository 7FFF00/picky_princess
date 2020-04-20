#ifndef AI_HPP
#define AI_HPP

class Ai {
public:
	class Entity* owner;
	int mode;
	bool active = false; // indicate if an entity is an active entity and in turn grabbable
	bool has_moved = false; // indicates if an entity has moved this turn or not
	int prev_x, prev_y; // keeps track of last movements made by entity
	bool engine_skip = false; // indicates if the engine's update loop should ignore
	bool turn_skip = false;	// to indicate if a turn is to be skipped
	virtual ~Ai() {}; // derived class, hence virtual destructor
	virtual void update() = 0;	// = 0 declares it as purely virtual
	void updateChild();
};

class PlayerAi : public Ai {
public:
	PlayerAi(Entity* e);
	void update();

protected:
	bool moveOrInteract(int target_x, int target_y);
	void handleKey(int ascii);
};

class EnemyAi : public Ai {
public:
	EnemyAi(Entity* e);
	void update();

protected:
	void moveOrInteract(int target_x, int target_y);
};

class ChildAi : public Ai {
public:
	ChildAi(Entity* e, Entity* p); // e for self, p for parent aka owner of entity
	void update();

protected:
};

#endif
