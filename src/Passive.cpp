#include <string>
#include <vector>							// for containers
#include "Entity.hpp"
#include "Engine.hpp"
#include "Gui.hpp"
#include "Stats.hpp"
#include "Passive.hpp"
#include <iostream>

Passive::Passive() {
  effect[DESCRIPTION] = "Basic Passive Skill";
}

Passive::~Passive() {

}

void Passive::update(Entity* user) {
}
