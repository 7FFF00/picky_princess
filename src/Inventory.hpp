#ifndef INVENTORY_HPP
#define INVENTORY_HPP
class Entity;

class Inventory {
public:
	int inv_size; // max inventory size, 0 is unlimited size, sum of invsize + eqpsize
	int primary_equip_size;
	int secondary_equip_size;

	std::vector<Entity> equipped;
	std::vector<Entity> held;
	std::vector<Entity> scrapped;
};

#endif
