#include "./fire.h"

#include <iostream>

#include "./dmg_types.h"


namespace goio {

bool Fire::burn(GoioObj* obj, double, bool&) {
  if (obj->get_fire_stacks() < 1)
    return false;
  obj->add_health(-firetick*dmg_types[DmgType::FIRE][obj->get_cmp_type()]*(2*obj->get_fire_stacks()+8));
  return true;
}

TimeFunc Fire::get_time_func(const GoioObj* obj, double, bool&) {
  if (obj->get_fire_stacks() > 0)
    return &Fire::get_firetick;
  return nullptr;
}

}  // namespace goio
