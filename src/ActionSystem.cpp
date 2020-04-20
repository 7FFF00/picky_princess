#include <string>
#include <sstream> // for taking a string stream to act as a text buffer, easy parsing of var
#include <vector>
#include <iostream>
#include "Entity.hpp"
#include "Ai.hpp"
#include "Engine.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Action.hpp"
#include "ActionSystem.hpp"

ActionSystem::ActionSystem() {
  char_ac_map['0'] = AC_NAME;
  char_ac_map['1'] = AC_TARGET;
  char_ac_map['2'] = AC_POWER;
  char_ac_map['3'] = AC_PICK;
  char_ac_map['4'] = AC_COST_TYPE;
  char_ac_map['5'] = AC_COST_AMOUNT;
  char_ac_map['6'] = AC_STATUS;
  char_ac_map['7'] = AC_RANGE;
  char_ac_map['8'] = AC_SPLASH;
  char_ac_map['9'] = AC_MESSAGE;
  char_ac_map['A'] = AC_DESCRIPTION;
  char_ac_map['B'] = AC_ENTITY_SHIFT;
  char_ac_map['C'] = AC_ENTITY_GENERATE;
  char_ac_map['D'] = AC_ENTITY_COMMAND;
  char_ac_map['E'] = AC_CONNECTED_ITEM;
  char_ac_map['F'] = AC_TRANSFORM_MAP;

  // EXAMPLE CONSTRUCTION, structured like this for
  // potential ease of reading from file
  //
  // std::vector<std::string> basic_components;
  // basic_components.push_back("0 Name: Skill Name!! Jungle Juice!!");
  // basic_components.push_back("9 Message: Skill Message Text!!");
  // basic_components.push_back("A Description: Skill Descript??");
  // createAction(basic_components);

  std::vector<std::string> grab;
  grab.push_back("0 Name: Grab");
  grab.push_back("3 Pick: True");
  createAction(grab);

  std::vector<std::string> chop;
  chop.push_back("0 Name: Chop");
  chop.push_back("2 Power: 1");
  createAction(chop);

  std::vector<std::string> stir;
  stir.push_back("0 Name: Stir");
  stir.push_back("B Entity Shift: 1");
  createAction(stir);

  std::vector<std::string> heat;
  heat.push_back("0 Name: Heat");
  stir.push_back("C Entity Generate: Fire");
  createAction(heat);

  std::vector<std::string> ice;
  ice.push_back("0 Name: Ice");
  createAction(ice);

  std::vector<std::string> eat;
  eat.push_back("0 Name: Eat");
  createAction(eat);

  std::vector<std::string> sniff;
  sniff.push_back("0 Name: Sniff");
  createAction(sniff);
}

ActionSystem::~ActionSystem() {}

void ActionSystem::createAction(std::vector<std::string> act_comps) {
  Action* new_act = new Action();
  std::string c_input, delimiter;
  delimiter = ":";
  for (int i = 0; i < (int)act_comps.size(); i++) {
    // X Field: FIELD_DATA_ENTRY
    c_input = act_comps[i].substr(act_comps[i].find(delimiter) + 2);

	// reading character at start of string from element of passed vector to determine where to
	// store the relevant data
    new_act->effect[char_ac_map[act_comps[i].at(0)]] = c_input;
  }
  engine.actions.push_back(new_act);
}

// void ActionSystem::modifyAction(Action act,
//   ActionComponent comp, std::string input) {
//
// }
//
void ActionSystem::update(Action* act, Entity* user, Entity* target) {
  // ACTION COMPOSITE SYSTEM to update by reading (potentially) queued
  // action calls by their passed and varied components, to allow for unique
  // combinations

  if (act->effect.find(AC_NAME) != act->effect.end()) {
    std::stringstream s;
    s << user->name << " used " << act->effect[AC_NAME] << "!";
    engine.gui->message("#3150d1", s);
  }

  if (act->effect.find(AC_PICK) != act->effect.end()) {
	// grab/add-to-inventory action
		// ATTEMPT ADD TO INVENTORY
		// ATTEMPT REACT ACTION
		// IF SUCCESSFUL ADD TO INVENTORY, ADD TARGET TO INVENTORY AND WHEN MC MOVES, ENTITY FOLLOWS
		if (act->effect[AC_PICK] == "True") {
			if (target->ai != NULL) {
				// target isn't immobile/grabbable yet
				engine.gui->message("#3150d1", "Can't grab!");
			}
			else {
				// target is grabbable
				target->ai = new ChildAi(target, user->findTail());
				engine.gui->message("#3150d1", "Grabbed!");
			}
		}
  }

  if (act->effect.find(AC_MESSAGE) != act->effect.end()) {
	engine.gui->message("#3150d1", act->effect[AC_MESSAGE]);
  }

  if (act->effect.find(AC_POWER) != act->effect.end()) {
    int power = std::stoi(act->effect[AC_POWER]);

  	std::stringstream s;
  	if (target->stats && !target->stats->isDead()) {
  		// if target has STATS module, and NOT dead, entity attacks target
		// damage math is ATK POWER - DELTA IN SIZE BETWEEN TARGET AND USER
  		s << user->name << " deals " << power << " to " << target->name << ".";
  		engine.gui->message("#404070", s);
  		target->stats->takeDamage(power - (target->size - user->size));
  	}
  	else {
  		// target is dead
  		s << target->name << " is dead, the attack is ineffective.";
  		engine.gui->message("#404070", s);
  	}
  }

  if (act->effect.find(AC_ENTITY_SHIFT) != act->effect.end()) {
	// engine.gui->message("#3150d1", act->effect[AC_MESSAGE]);
  }

  if (act->effect.find(AC_ENTITY_GENERATE) != act->effect.end()) {
	// engine.gui->message("#3150d1", act->effect[AC_MESSAGE]);
  }
}
//
// void ActionSystem::update(Action act, Entity* user, int x, int y) {}
//
// void ActionSystem::render() {}
