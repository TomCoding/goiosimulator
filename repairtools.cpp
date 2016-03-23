#include "repairtools.h"
#include "goioobj.h"

#include <cassert>

#include <iostream>
#include <iomanip>

namespace goio {

  void RepairTool::set_cur_swing(double swing) {
    if (swing > get_swing())
      cur_swing = get_swing();
    else if (swing < 0)
      cur_swing = 0;
    else
      cur_swing = swing;
  }

  bool RepairTool::repair(GoioObj* obj) {
    cur_swing = get_swing();
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
    cur_swing = get_swing();
  }

  TimeFunc RepairTool::get_time_func(const GoioObj* obj, double time, bool& force) {
    switch(done) {
      case 1:
        return std::bind(&RepairTool::get_cur_swing, this);
      case 2:
        return nullptr;
      case 0:
        if (obj->get_health() == 0) {
          auto timediff = get_cur_swing() - time;
          // still in swing or ready? (tool usage ahead of time)
          if (timediff > 0)
            cur_swing = timediff;
          else
            // slightly delayed, to prevent possible insta-rebuilds
            cur_swing = swing_foreshadowing_delay;
          force = true;
          return std::bind(&RepairTool::get_cur_swing, this);
        }
        return std::bind(&RepairTool::get_cooldown, this);
      default:
        assert(false);
    }
  }

}
