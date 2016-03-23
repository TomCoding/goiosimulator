#include "engineer.h"
#include "goioobj.h"
#include <string>

#include <cassert>

namespace goio {

  Engineer::Engineer(const std::string name, RepairMode mode) :
                     GoioActor::GoioActor(name, CmpType::HULL),
                     mode(mode), cur_tool(nullptr) {
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
    if (cur_tool != nullptr)
      cur_tool->reset();
    cur_tool = tool;
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

    if (obj->get_health() == 0) {
      auto timediff = cur_tool->get_cur_swing() - time;
      if (timediff > 0)
        cur_tool->set_cur_swing(timediff);
      else
        cur_tool->set_cur_swing(RepairTool::swing_foreshadowing_delay);
      force = true;
    }

    return cur_tool->get_time_func(obj, time, force);
  }
}
