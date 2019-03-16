#ifndef PASSIVE_HPP
#define PASSIVE_HPP
#include <map>

class Passive {
public:
  enum PassiveComponent {
      XP_ABS, XP_PCT,
      MHP_ABS, MHP_PCT, HP_REGEN_ABS, HP_REGEN_PCT,
      MEN_ABS, MEN_PCT, EN_REGEN_ABS, EN_REGEN_PCT,
      MFU_ABS, MFU_PCT, FU_REGEN_ABS, FU_REGEN_PCT,
      MCH_ABS, MCH_PCT, CH_REGEN_ABS, CH_REGEN_PCT,
      TANK_CAPACITY_ABS, TANK_CAPACITY_PCT,
      DAMAGE,
      SIGHT,
      STR, VIT, INT, DEX, PER,
      KIN, PSY, THERMO, ELEC, HYDRO, CHEM, BIO,
      STATUS_IMMUNITY, STATUS_DURATION,
      ARMOR_FLAG,
      REQUIREMENT, SKILL_CAST,
      TRADE,
      DESCRIPTION,
  };
  // Passive composition system
	// std::string name; // entity name
  std::map<PassiveComponent, std::string> effect; // collection of components

  Passive();
	~Passive();

  void update(Entity* user);
};

#endif
