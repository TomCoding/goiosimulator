#include "engineer.h"
#include "goioobj.h"
#include <string>

#include <cassert>

#include <iostream>

namespace goio {

  Engineer::Engineer(const std::string name, RepairTool* tool1, RepairTool* tool2,
                     RepairTool* tool3, RepairMode mode) :
                     GoioActor::GoioActor(name, CmpType::HULL),
                     mode(mode), tools(), repair_treshholds(),
                     max_rep_tools(), rebuild_tools(), rebuild_treshholds(),
                     cur_tool(nullptr), delay(false) {
    update_tools(tool1, tool2, tool3);
  }

  void Engineer::update_tools(RepairTool* tool1, RepairTool* tool2, RepairTool* tool3) {
    for (auto i = 0; i < 3; ++i) {
      double ps1;
      double ps2;
      double ps3;
      RepairTool* *ptr_tools;

      switch (i) {
        case 0:
          ps1 = tool1->get_heal()/tool1->get_cooldown();
          ps2 = tool2->get_heal()/tool2->get_cooldown();
          ps3 = tool3->get_heal()/tool3->get_cooldown();
          ptr_tools = tools;
          break;
        case 1:
          ps1 = tool1->get_heal();
          ps2 = tool2->get_heal();
          ps3 = tool3->get_heal();
          ptr_tools = max_rep_tools;
          break;
        case 2:
          ps1 = tool1->get_rebuild_power()/tool1->get_swing();
          ps2 = tool2->get_rebuild_power()/tool2->get_swing();
          ps3 = tool3->get_rebuild_power()/tool3->get_swing();
          ptr_tools = rebuild_tools;
          break;
        default:
          assert(false);
      }

      if (ps1 > ps2) {
        if (ps2 > ps3) {
          ptr_tools[0] = tool1;
          ptr_tools[1] = tool2;
          ptr_tools[2] = tool3;
        } else if (ps1 > ps3) {
          ptr_tools[0] = tool1;
          ptr_tools[1] = tool3;
          ptr_tools[2] = tool2;
        } else {
          ptr_tools[0] = tool3;
          ptr_tools[1] = tool1;
          ptr_tools[2] = tool2;
        }
      } else {
        if (ps1 > ps3) {
          ptr_tools[0] = tool2;
          ptr_tools[1] = tool1;
          ptr_tools[2] = tool3;
        } else if (ps2 > ps3) {
          ptr_tools[0] = tool2;
          ptr_tools[1] = tool3;
          ptr_tools[2] = tool1;
        } else {
          ptr_tools[0] = tool3;
          ptr_tools[1] = tool2;
          ptr_tools[2] = tool1;
        }
      }
    }

    repair_treshholds[0] = tools[1]->get_heal()*tools[0]->get_cooldown()/tools[1]->get_cooldown();
    repair_treshholds[1] = tools[2]->get_heal()*tools[1]->get_cooldown()/tools[2]->get_cooldown();
    rebuild_treshholds[0] = rebuild_tools[1]->get_rebuild_power()*rebuild_tools[0]->get_swing()/tools[1]->get_swing();
    rebuild_treshholds[1] = rebuild_tools[2]->get_rebuild_power()*rebuild_tools[1]->get_swing()/tools[2]->get_swing();
  }

  void Engineer::select_tool(RepairTool* tool) {
    if (tool == nullptr) {
      tool[0].reset();
      tool[1].reset();
      tool[2].reset();
      cur_tool = nullptr;
      return;
    } else if (tool == cur_tool) {
      delay = false;
      return;
    }

    double swing;
    bool set_swing;
    if (cur_tool != nullptr) {
      if (cur_tool == &tool[0]) {
        tool[1].reset();
        tool[2].reset();
      } else if (cur_tool == &tool[1]) {
        tool[0].reset();
        tool[2].reset();
      } else if (cur_tool == &tool[2]) {
        tool[0].reset();
        tool[1].reset();
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
    if (obj->get_health() == 0) {
      auto rebuilddiff = obj->get_rebuild_value() - obj->get_rebuild_state();
      if (rebuilddiff > rebuild_treshholds[0])
        select_tool(rebuild_tools[0]);
      else if (rebuilddiff > rebuild_treshholds[1])
        select_tool(rebuild_tools[1]);
      else
        select_tool(rebuild_tools[2]);
    } else {
      auto healthdiff = obj->get_max_health() - obj->get_health();
      switch(mode) {
        case RepairMode::CONSTANT_DMG_NO_WAIT:
          if (healthdiff > repair_treshholds[0])
            select_tool(tools[0]);
          else if (healthdiff > repair_treshholds[1])
            select_tool(tools[1]);
          else
            select_tool(tools[2]);
          break;
        case RepairMode::FASTEST_HEAL:
          if (healthdiff > tools[2]->get_heal()) {
            if (healthdiff > tools[1]->get_heal())
              select_tool(tools[0]);
            else
              select_tool(tools[1]);
          } else
            select_tool(tools[2]);
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

  void Engineer::free_tools() {
    for (auto i = 0; i < 3; ++i)
      delete tools[i];
  }

  MainEngineer::MainEngineer(const std::string name, bool extinguisher, RepairMode mode) :
                             Engineer(name, mode) {
    RepairTool* firetool;
    if (extinguisher)
      firetool = new FireExtinguisher(name + " Extinguiser");
    else
      firetool = new ChemicalSpray(name + " Chem");
    update_tools(new Mallet(name + " Mallet"), new Spanner(name + " Spanner"), firetool);
  }

  MainEngineer::~MainEngineer() {
    free_tools();
  }

  BuffEngineer::BuffEngineer(const std::string name, bool extinguisher) :
                             Engineer(name, RepairMode::CONSTANT_DMG_NO_WAIT) {
    RepairTool* firetool;
    if (extinguisher)
      firetool = new FireExtinguisher(name + " Extinguiser");
    else
      firetool = new ChemicalSpray(name + " Chem");
    update_tools(new PipeWrench(name + " Wrench"), new BuffHammer(name + " Buff"), firetool);
  }

  BuffEngineer::~BuffEngineer() {
    free_tools();
  }
}
