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

  bool RepairTool::repair(GoioObj* obj, double time, bool&) {
    cur_swing = get_swing();
    if (obj->get_health() == obj->get_max_health() ||
              (obj->get_health() == 0 && obj->get_hull()->get_health() == 0)) {
      done = 2;
      // if (get_name() != "Wrench2")
      //   std::cout << "\ndone set to 2: " << get_name() << std::endl << "        ";
      std::cout << "                            ";
      return false;
    } else if (obj->get_cooldown_end() > time) {
      done = 0;
      repair_wait = obj->get_cooldown_end() - time;
      // if (get_name() != "Wrench2")
      //   std::cout << "\ndone set to 0: " << get_name()
      //             << " repair_wait: " << repair_wait << std::endl << "        ";
      std::cout << "                            ";
      return false;
    }
    if (done == 2) {
      done = 1;  // start swing
      // cur_swing = swing_foreshadowing_delay;
      // std::cout << "\ndone set to 1: " << get_name() << std::endl << "        ";
      std::cout << "                            ";
      return false;
    }

    using namespace std;
    cout << fixed << setprecision(2);
    // cout << "\ncooldownR: " << obj->get_name() << " "
    //      << obj->get_cooldown_end() << endl << "        ";
    cout << setw(15) << right << get_name();
    cout << "             ";

    if (obj->get_health() > 0) {
      done = 0;
      obj->add_health(get_heal(), time + get_cooldown());
      repair_wait = get_cooldown();
    } else {
      done = 1;
      obj->add_rebuild(get_rebuild_power());
      repair_wait = 0;
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
        if (obj->get_health() == obj->get_max_health())
          return nullptr;

        if (obj->get_health() == 0) {
          if (obj->get_hull()->get_health() == 0)
            return nullptr;
          cur_swing = swing_foreshadowing_delay;
          force = true;
          return std::bind(&RepairTool::get_cur_swing, this);
        }

        return std::bind(&RepairTool::get_cooldown, this);
      case 0:
        if (obj->get_health() == 0) {
          auto timediff = get_cur_swing() - time;
          // still in swing or ready? (tool usage ahead of time)
          if (timediff >= -0.00000001)  // take care of floating point errors
            cur_swing = timediff;
          else
            cur_swing = swing_foreshadowing_delay;
          // std::cout << "\nset swing delay: "
          //           << cur_swing << " "
          //           << get_name() << std::endl
          //           << "                                    ";
          force = true;
          repair_wait = 0;
          return std::bind(&RepairTool::get_cur_swing, this);
        }

        if (wait_cooldown() > 0)
          return std::bind(&RepairTool::wait_cooldown, this);
        else
          return std::bind(&RepairTool::get_cooldown, this);
      default:
        assert(false);
    }
  }

}
