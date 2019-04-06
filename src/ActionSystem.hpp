#ifndef ACTION_SYSTEM_HPP
#define ACTION_SYSTEM_HPP
#include <map>

class ActionSystem {
public:
  // Action management system
  std::map<char, ActionComponent> char_ac_map; // 0 = AC_NAME, A = AC_DESCR etc

  ActionSystem();
	~ActionSystem();
  void createAction(std::vector<std::string> act_comps);
  // void modifyAction(Action act, ActionComponent comp, std::string input);
  void update(Action* act, Entity* user, Entity* target);
  // void update(Action act, Entity* user, int x, int y);
  // void render();
};

#endif
