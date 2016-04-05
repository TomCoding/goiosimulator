/**
 * goiovalues - Guns of Icarus Online damage simulator
 * Copyright (C) 2016  Dominique Lasserre
 *
 * This file is part of goiovalues.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "./engineer.h"

#include <string>
#include <cassert>

#include <iostream>

#include "./goioobj.h"
#include "./fire.h"


namespace goio {

Engineer::Engineer(const std::string& name, RepairTool* tool1,
                   RepairTool* tool2, RepairTool* tool3,
                   RepairMode mode, ExtinguishMode extmode) :
                   GoioActor::GoioActor(name, CmpType::HULL),
                   mode(mode), extmode(extmode), tools(), repair_thresholds(),
                   max_rep_tools(), rebuild_tools(), rebuild_thresholds(),
                   cur_tool(nullptr), delay(false) {
  update_tools(tool1, tool2, tool3);
}

void Engineer::update_tools(RepairTool* tool1, RepairTool* tool2,
                            RepairTool* tool3) {
  for (auto i = 0; i < 4; ++i) {
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
      case 3:
        ps1 = tool1->get_extinguish()/tool1->get_cooldown();
        ps2 = tool2->get_extinguish()/tool2->get_cooldown();
        ps3 = tool3->get_extinguish()/tool3->get_cooldown();
        ptr_tools = ext_tools;
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

  repair_thresholds[0] = tools[1]->get_heal() *
                         tools[0]->get_cooldown()/tools[1]->get_cooldown();
  repair_thresholds[1] = tools[2]->get_heal() *
                         tools[1]->get_cooldown()/tools[2]->get_cooldown();
  rebuild_thresholds[0] = rebuild_tools[1]->get_rebuild_power() *
                          rebuild_tools[0]->get_swing()/tools[1]->get_swing();
  rebuild_thresholds[1] = rebuild_tools[2]->get_rebuild_power() *
                          rebuild_tools[1]->get_swing()/tools[2]->get_swing();
}

void Engineer::select_tool(RepairTool* tool) {
  if (tool == cur_tool) {
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
    } else {
      swing = cur_tool->get_cur_swing()/cur_tool->get_swing()*tool->get_swing();
    }
    set_swing = true;
  } else {
    set_swing = false;
  }

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
    if (rebuilddiff > rebuild_thresholds[0])
      select_tool(rebuild_tools[0]);
    else if (rebuilddiff > rebuild_thresholds[1])
      select_tool(rebuild_tools[1]);
    else
      select_tool(rebuild_tools[2]);
  } else {
    if (obj->get_fire_stacks() > 0 && ext_tools[0]->get_extinguish() > 0) {
      switch (extmode) {
        case ExtinguishMode::INSTANT:
          select_tool(ext_tools[0]);
          return cur_tool->repair(obj, time, changed);
        case ExtinguishMode::THRESHOLD:
          if (tools[0]->get_heal()/tools[0]->get_cooldown() <
              Fire::get_fire_dmg(obj, tools[0]->get_cooldown())) {
            select_tool(ext_tools[0]);
            return cur_tool->repair(obj, time, changed);
          }
          break;
        default:
          assert(false);
      }
    }

    auto healthdiff = obj->get_max_health() - obj->get_health();
    switch (mode) {
      case RepairMode::CONSTANT_DMG_NO_WAIT:
        if (healthdiff > repair_thresholds[0])
          select_tool(tools[0]);
        else if (healthdiff > repair_thresholds[1])
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
        } else {
          select_tool(tools[2]);
        }
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
  switch (cur_tool->get_done()) {
    case 2:
        if (obj->get_health() == 0 && obj->get_hull()->get_health() != 0)
          delay = true;
      break;
    case 0:
        if (obj->get_health() == 0 &&
                              cur_tool->get_cur_swing() - time < -0.00000001)
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

MainEngineer::MainEngineer(const std::string& name, bool extinguisher,
                           RepairMode mode, ExtinguishMode extmode) :
                           Engineer(name, mode, extmode) {
  RepairTool* firetool;
  if (extinguisher)
    firetool = new FireExtinguisher(name + " Ext");
  else
    firetool = new ChemicalSpray(name + " Chem");
  update_tools(new Mallet(name + " Mallet"),
               new Spanner(name + " Spanner"),
               firetool);
}

MainEngineer::~MainEngineer() {
  free_tools();
}

BuffEngineer::BuffEngineer(const std::string& name, bool extinguisher,
                           ExtinguishMode extmode) :
                           Engineer(name,
                                    RepairMode::CONSTANT_DMG_NO_WAIT,
                                    extmode) {
  RepairTool* firetool;
  if (extinguisher)
    firetool = new FireExtinguisher(name + " Ext");
  else
    firetool = new ChemicalSpray(name + " Chem");
  update_tools(new PipeWrench(name + " Wrench"),
               new BuffHammer(name + " Buff"),
               firetool);
}

BuffEngineer::~BuffEngineer() {
  free_tools();
}

}  // namespace goio
