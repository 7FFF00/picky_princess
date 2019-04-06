#include <string>
#include <vector>
#include <iostream>
#include "Entity.hpp"
#include "Engine.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Action.hpp"
#include "ActionSystem.hpp"

Action::Action() {
}

Action::~Action() {

}

void Action::update(Entity* user, Entity* target) {
  engine.as->update(this, user, target);
  // user->stats->attack(target);
  // std::string s = "";
  // s += user->name;
  // s += " used ";
  // s += effect[AC_NAME];
  // engine.gui->message("#3150d1", s);
  // if (effect.find(AC_MESSAGE) != effect.end()) {
  //   // std::map::find returns an iterator either to the found entry, or if not
  //   // found, then it returns an iterator to map::end instead
  //   engine.gui->message("#3150d1", effect[AC_MESSAGE]);
  // }
}

void Action::render() {

}
