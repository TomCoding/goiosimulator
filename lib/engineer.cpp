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

REGISTER_TYPE_FULLREP(MainEngineer);
REGISTER_TYPE_FULLREP(BuffEngineer);

Engineer::Engineer(const std::string& name, RepairTool* tool1,
                   RepairTool* tool2, RepairTool* tool3,
                   RepairMode mode, ExtinguishMode extmode) :
                   RepairActor(name, CmpType::HULL),
                   mode(mode), extmode(extmode), tools(), repair_thresholds(),
                   max_rep_tools(), rebuild_tools(), rebuild_thresholds(),
                   cur_tool(nullptr), delay(false) {
  update_tools(tool1, tool2, tool3);
}

void Engineer::update_tools(RepairTool* tool1, RepairTool* tool2,
                            RepairTool* tool3) {
  set_tools(tool1->get_heal()/tool1->get_cooldown(),
            tool2->get_heal()/tool2->get_cooldown(),
            tool3->get_heal()/tool3->get_cooldown(),
            tool1,
            tool2,
            tool3,
            tools);
  set_tools(tool1->get_heal(),
            tool2->get_heal(),
            tool3->get_heal(),
            tool1,
            tool2,
            tool3,
            max_rep_tools);
  set_tools(tool1->get_rebuild_power()/tool1->get_swing(),
            tool2->get_rebuild_power()/tool2->get_swing(),
            tool3->get_rebuild_power()/tool3->get_swing(),
            tool1,
            tool2,
            tool3,
            rebuild_tools);
  set_tools((tool1->get_extinguish() > max_fire_stacks) ?
                max_fire_stacks/tool1->get_cooldown() :
                tool1->get_extinguish()/tool1->get_cooldown(),
            (tool2->get_extinguish() > max_fire_stacks) ?
                max_fire_stacks/tool2->get_cooldown() :
                tool2->get_extinguish()/tool2->get_cooldown(),
            (tool3->get_extinguish() > max_fire_stacks) ?
                max_fire_stacks/tool3->get_cooldown() :
                tool3->get_extinguish()/tool3->get_cooldown(),
            tool1,
            tool2,
            tool3,
            ext_tools);

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

  Time swing;
  bool set_swing;
  if (cur_tool != nullptr) {
    if (cur_tool == &tool[0]) {
      tools[1]->reset();
      tools[2]->reset();
    } else if (cur_tool == &tool[1]) {
      tools[0]->reset();
      tools[2]->reset();
    } else if (cur_tool == &tool[2]) {
      tools[0]->reset();
      tools[1]->reset();
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

RepairTool* Engineer::get_repair_tool(GoioObj* obj) {
  auto healthdiff = obj->get_max_health() - obj->get_health();
  switch (mode) {
    case RepairMode::CONSTANT_DMG_NO_WAIT:
      if (healthdiff > repair_thresholds[0])
        return tools[0];
      else if (healthdiff > repair_thresholds[1])
        return tools[1];
      else
        return tools[2];
    case RepairMode::FASTEST_HEAL:
      if (healthdiff > tools[2]->get_heal()) {
        if (healthdiff > tools[1]->get_heal())
          return tools[0];
        else
          return tools[1];
      } else {
        return tools[2];
      }
    default:
      assert(false);
  }
}

DmgState Engineer::repair(GoioObj* obj, Time time) {
  if (obj->get_health() == 0_hp) {
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
        case ExtinguishMode::THRESHOLD:
          {
          auto repair_tool = get_repair_tool(obj);
          if (repair_tool->get_heal() <
                      Fire::get_fire_dmg(obj, repair_tool->get_cooldown())) {
            select_tool(ext_tools[0]);
          } else {
            select_tool(repair_tool);
          }
          }
          return cur_tool->repair(obj, time);
        case ExtinguishMode::INSTANT:
          select_tool(ext_tools[0]);
          return cur_tool->repair(obj, time);
        default:
          assert(false);
      }
    }

    select_tool(get_repair_tool(obj));
  }

  return cur_tool->repair(obj, time);
}

TimeFunc Engineer::get_time_func(const GoioObj* obj, Time time, bool& force) {
  if (cur_tool == nullptr)
    return nullptr;
  switch (cur_tool->get_done()) {
    case 2:
        if (obj->get_health() == 0_hp && obj->get_hull()->get_health() != 0_hp)
          delay = true;
      break;
    case 0:
        if (obj->get_health() == 0_hp &&
                            cur_tool->get_cur_swing() - time < -0.00000001_s)
          delay = true;
      break;
    default:
      break;
  }
  return cur_tool->get_time_func(obj, time, force);
}

void Engineer::free_tools() {
  for (auto&& tool : tools)
    delete tool;
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
                           RepairMode mode, ExtinguishMode extmode) :
                           Engineer(name, mode, extmode) {
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
