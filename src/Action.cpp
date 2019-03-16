#include <string>
#include <vector>
#include <iostream>
#include "Entity.hpp"
#include "Engine.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Action.hpp"

Action::Action() {
}

Action::~Action() {

}

void Action::update(Entity* user, Entity* target) {
  user->stats->attack(target);
  std::string s = "";
  s += user->name;
  s += " used ";
  s += effect[AC_NAME];
  engine.gui->message("#3150d1", s);
  engine.gui->message("#3150d1", effect[AC_MESSAGE]);
}

void Action::render() {

}
