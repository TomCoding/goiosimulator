#include "fire.h"
#include "dmg_types.h"

#include <iostream>

namespace goio {

  bool Fire::burn(GoioObj* obj, double, bool&) {
    if (obj->get_fire_stacks() < 1)
      return false;
    obj->add_health(-firetick*dmg_types[DmgType::FIRE][obj->get_cmp_type()]*(2*obj->get_fire_stacks()+8));
    return true;
  }

  TimeFunc Fire::get_time_func(const GoioObj* obj, double, bool&) {
    if (obj->get_fire_stacks() > 0)
      return std::bind(&Fire::get_firetick, this);
    return nullptr;
  }

}
