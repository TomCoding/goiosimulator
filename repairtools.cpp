#include "repairtools.h"
#include "goioobj.h"

#include <iostream>
#include <iomanip>

namespace goio {

  bool RepairTool::repair(GoioObj* obj) {
    if (obj->get_health() == obj->get_max_health() ||
        (obj->get_health() == 0 && obj->get_hull()->get_health() == 0)) {
      done = 2;
      std::cout << "                            ";
      return false;
    }

    using namespace std;
    cout << fixed << setprecision(2);
    cout << setw(15) << right << get_name();
    cout << "             ";

    if (obj->get_health() > 0) {
      done = 0;
      obj->add_health(get_heal());
    } else {
      done = 1;
      obj->add_rebuild(get_rebuild_power());
    }

    return true;
  }

  void RepairTool::reset(bool) {
    done = 0;
  }

  TimeFunc RepairTool::get_time_func(const GoioObj* obj, double time) {
    switch (done) {
      case 1:
        return std::bind(&RepairTool::get_swing, this);
      case 2:
        return nullptr;
      default:
        return std::bind(&RepairTool::get_cooldown, this);
    };
  }

}
