#include "./monitor.h"

#include <iostream>

#include "./dmg_types.h"


namespace goio {

bool Monitor::monitor(GoioObj* obj, double, bool&) {
  std::cout << "                            ";
  if (obj->get_health() == 0 && obj->get_hull()->get_health() == 0)
    return false;
  return true;
}

TimeFunc Monitor::get_time_func(const GoioObj* obj, double, bool&) {
  if (obj->get_health() == 0 && obj->get_hull()->get_health() == 0)
    return nullptr;
  return std::bind(&Monitor::get_tick, this);
}

}  // namespace goio
