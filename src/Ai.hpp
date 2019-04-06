#ifndef AI_HPP
#define AI_HPP

class Ai {
public:
	class Entity* owner;
	int mode;
	bool turn_skip = false;	// to indicate if a turn is to be skipped
	virtual ~Ai() {}; // derived class, hence virtual destructor
	virtual void update() = 0;	// = 0 declares it as purely virtual
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

#endif
