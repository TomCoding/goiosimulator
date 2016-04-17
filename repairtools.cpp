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

#include "./repairtools.h"

#include <cassert>

#include <iostream>
#include <iomanip>

#include "./goioobj.h"


namespace goio {

REGISTER_TYPE(Spanner, "Spanner");
REGISTER_TYPE(Mallet, "Mallet");
REGISTER_TYPE(PipeWrench, "PipeWrench");
REGISTER_TYPE(FireExtinguisher, "FireExtinguisher");
REGISTER_TYPE(ChemicalSpray, "ChemicalSpray");
REGISTER_TYPE(BuffHammer, "BuffHammer");

void RepairTool::set_cur_swing(double swing) {
  if (swing > get_swing())
    cur_swing = get_swing();
  else if (swing < 0)
    cur_swing = 0;
  else
    cur_swing = swing;
}

DmgState::State RepairTool::repair(GoioObj* obj, double time) {
  cur_swing = get_swing();

  if (obj->get_health() == 0 && obj->get_hull()->get_health() == 0) {
    done = 2;
    std::cout << "                            ";
    return DmgState::NONE;
  }

  auto valid = true;
  if (get_heal() > 0 && obj->get_health() == obj->get_max_health())
    valid = false;
  if ((!valid || get_heal() == 0) && (get_extinguish() == 0 ||
                    (get_extinguish() > 0 && obj->get_fire_stacks() <= 0))) {
    done = 2;
    std::cout << "                            ";
    return DmgState::NONE;
  } else if (obj->get_cooldown_end() > time) {
    done = 0;
    repair_wait = obj->get_cooldown_end() - time;
    // if (get_name() != "Wrench2")
    //   std::cout << "\ndone set to 0: " << get_name()
    //             << " repair_wait: " << repair_wait << std::endl
    //             << "        ";
    std::cout << "                            ";
    return DmgState::NONE;
  }
  if (done == 2) {
    done = 1;  // start swing
    // cur_swing = swing_foreshadowing_delay;
    // std::cout << "\ndone set to 1: " << get_name() << std::endl
    //           << "        ";
    std::cout << "                            ";
    return DmgState::NONE;
  }

  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;
  using std::setw;
  using std::right;
  cout << fixed << setprecision(2);
  // cout << "\ncooldownR: " << obj->get_name() << " "
  //      << obj->get_cooldown_end() << endl << "        ";
  cout << setw(15) << right << get_name();
  cout << "             ";

  auto ret = DmgState::NONE;
  if (obj->get_health() > 0) {
    done = 0;
    obj->add_health(get_heal(), time + get_cooldown());
    if (get_heal() > 0)
      ret |= DmgState::TARGET;
    double immunity_end;
    auto start_immunity = obj->get_immunity_end() < time;
    if (time + get_fire_immune() > obj->get_immunity_end()) {
      immunity_end = time + get_fire_immune();
      if (start_immunity)
        ret |= DmgState::START_IMMUNITY;
      else
        ret |= DmgState::IMMUNITY;
    } else {
      immunity_end = -1;
    }
    obj->add_fire(-get_extinguish(), immunity_end);
    if (get_extinguish() > 0)
      ret |= DmgState::FIRE;
    repair_wait = get_cooldown();
  } else {
    done = 1;
    auto start_rebuild = obj->get_rebuild_state() == 0;
    if (obj->add_rebuild(get_rebuild_power()))
      ret |= DmgState::TRANSITIONED;
    else if (get_rebuild_power() > 0) {
      if (start_rebuild)
        ret |= DmgState::START_REBUILD;
      else
        ret |= DmgState::REBUILD;
    }
    repair_wait = 0;
  }

  return ret;
}

void RepairTool::reset(bool) {
  done = 0;
  cur_swing = get_swing();
}

TimeFunc RepairTool::get_time_func(const GoioObj* obj, double time, bool& force) {
  switch (done) {
    case 1:
      return std::bind(&RepairTool::get_cur_swing, this);
    case 2:
      {
      auto valid = true;
      if (get_heal() > 0 && obj->get_health() == obj->get_max_health())
        valid = false;
      if ((!valid || get_heal() == 0) && (get_extinguish() == 0 ||
                      (get_extinguish() > 0 && obj->get_fire_stacks() <= 0)))
          return nullptr;
      }

      if (obj->get_health() == 0) {
        if (obj->get_hull()->get_health() == 0)
          return nullptr;
        if (get_rebuild_power() == 0)
          return nullptr;
        cur_swing = swing_foreshadowing_delay;
        force = true;
        return std::bind(&RepairTool::get_cur_swing, this);
      }

      return std::bind(&RepairTool::get_cooldown, this);
    case 0:
      if (obj->get_health() == 0) {
        // if (get_rebuild_power() == 0) {
        //   done = 2;
        //   return nullptr;
        // }

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

}  // namespace goio
