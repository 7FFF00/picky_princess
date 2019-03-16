#ifndef ACTION_HPP
#define ACTION_HPP
#include <map>

enum ActionEvent {
  // defines possible causes for Actions, where CAST is the basic use clause
  // for an active action
  EVENT_AUTO,           // passive effects, as they are automatically cast
  EVENT_CAST,           // on casting this Action
  EVENT_CONNECTED_CAST, // casting a connected Action
  EVENT_ANY_CAST,       // casting anything
  EVENT_SPAWN,          // on entity spawn
  EVENT_MOVE,           // on moving at all
  EVENT_BUMP,           // on moving into a blocking entity
  EVENT_KNOCKBACK,      // on being knocked back
  EVENT_DEAL_DAMAGE,    // on dealing damage
  EVENT_TAKE_DAMAGE,    // on taking damage
  EVENT_HIT,            // on hitting something at all
  EVENT_DEATH,          // on something dying
  EVENT_KILL,           // on successful kill
  EVENT_STATUS,         // on successful status inflict
  EVENT_TIMER_END       // on a given timer ending
};

enum ActionComponent {
  AC_NAME,              // action name
  AC_TARGET,            // KEY COMPONENT, action target type, ie SELF/Ground/etc
  AC_POWER,             // action potency/strength
  AC_COST_TYPE,         // resource type, HP/EN/CH/Fuel/Items/etc
  AC_COST_AMOUNT,       // resource cost
  AC_ELEMENT,           // base damage type
  AC_STATUS,            // attached status infliction, if present
  AC_RANGE,             // action targetting range
  AC_SPLASH,            // aoe splash radius
  AC_MESSAGE,           // action use message for player log
  AC_DESCRIPTION,       // action popup description when inspected
  AC_ENTITY_SHIFT,      // tile knockback or entity position shifting
  AC_ENTITY_GENERATE,   // generate an entity, a spark, fire, monster, etc
  AC_ENTITY_COMMAND,    // send command for target entity to perform
  AC_CONNECTED_ITEM,    // instance of an item a particular action points to
  AC_TRANSFORM_MAP,     // map transformation, tile changing, wall digging etc
};

class Action {
public:
  // Action composition system
  ActionEvent cause;  // collection of possible reasons for Action to take place, typically only 1
  std::map<ActionComponent, std::string> effect; // collection of components

  Action();
	~Action();

  void update(Entity* user, Entity* target);
  void render();
};

#endif
