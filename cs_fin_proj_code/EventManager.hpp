#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "Components.hpp"

class EventManager {
public:
  static void TriggerAgeEvent(Character& c, Player& pl, int birth_type);
};

#endif