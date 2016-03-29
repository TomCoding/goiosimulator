#include "engineer.h"
#include "goioobj.h"
#include <string>

#include <cassert>

#include <iostream>

namespace goio {

  Engineer::Engineer(const std::string name, RepairMode mode) :
                     GoioActor::GoioActor(name, CmpType::HULL),
                     mode(mode), cur_tool(nullptr), delay(false) {
    std::string tmpname;
    if (name.length() == 0)
      tmpname = "Engineer ";
    else
      tmpname = name + " ";
    mallet = new Mallet(tmpname + "Mallet");
    wrench = new PipeWrench(tmpname + "Wrench");
    spanner = new Spanner(tmpname + "Spanner");

    mallet_treshhold = wrench->get_heal()*mallet->get_cooldown()/wrench->get_cooldown();
    wrench_treshhold = spanner->get_heal()*wrench->get_cooldown()/spanner->get_cooldown();
  }

  Engineer::~Engineer() {
    delete mallet;
    delete wrench;
    delete spanner;
  }

  void Engineer::select_tool(RepairTool* tool) {
    if (tool == nullptr) {
      mallet->reset();
      wrench->reset();
      spanner->reset();
      cur_tool = nullptr;
      return;
    } else if (tool == cur_tool) {
      delay = false;
      return;
    }

    double swing;
    bool set_swing;
    if (cur_tool != nullptr) {
      if (cur_tool == mallet) {
        wrench->reset();
        spanner->reset();
      } else if (cur_tool == wrench) {
        mallet->reset();
        spanner->reset();
      } else if (cur_tool == spanner) {
        mallet->reset();
        wrench->reset();
      }
      if (delay) {
        swing = cur_tool->get_cur_swing();
        delay = false;
      } else
        swing = cur_tool->get_cur_swing()/cur_tool->get_swing()*tool->get_swing();
      set_swing = true;
    } else
      set_swing = false;

    cur_tool = tool;
    if (set_swing)
      cur_tool->set_cur_swing(swing);
    // std::cout << std::endl << cur_tool->get_name()
    //           << " " << cur_tool->get_done() << " "
    //           << cur_tool->get_cur_swing() << std::endl << "        ";
  }

  bool Engineer::repair(GoioObj* obj, double time, bool& changed) {
    if (obj->get_health() == 0)
      select_tool(spanner);
    else {
      auto healthdiff = obj->get_max_health() - obj->get_health();
      switch(mode) {
        case RepairMode::CONSTANT_DMG_NO_WAIT:
          if (healthdiff > mallet_treshhold)
            select_tool(mallet);
          else if (healthdiff > wrench_treshhold)
            select_tool(wrench);
          else
            select_tool(spanner);
          break;
        case RepairMode::FASTEST_HEAL:
          if (healthdiff > spanner->get_heal()) {
            if (healthdiff > wrench->get_heal())
              select_tool(mallet);
            else
              select_tool(wrench);
          } else
            select_tool(spanner);
          break;
        default:
          assert(false);
      }
    }

    return cur_tool->repair(obj, time, changed);
  }

  TimeFunc Engineer::get_time_func(const GoioObj* obj, double time, bool& force) {
    if (cur_tool == nullptr)
      return nullptr;
    switch(cur_tool->get_done()) {
      case 2:
          if (obj->get_health() == 0 && obj->get_hull()->get_health() != 0)
            delay = true;
        break;
      case 0:
          if (obj->get_health() == 0 && cur_tool->get_cur_swing() - time < -0.00000001)
            delay = true;
        break;
      default:
        break;
    }
    return cur_tool->get_time_func(obj, time, force);
  }
}
