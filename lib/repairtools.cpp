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

const EnginesBuff BuffTool::engines_buff = EnginesBuff();
const GunsBuff BuffTool::guns_buff = GunsBuff();
const BalloonBuff BuffTool::balloon_buff = BalloonBuff();

REGISTER_TYPE(Spanner);
REGISTER_TYPE(Mallet);
REGISTER_TYPE(PipeWrench);
REGISTER_TYPE(FireExtinguisher);
REGISTER_TYPE(ChemicalSpray);
REGISTER_TYPE(BuffHammer);

void RepairTool::set_cur_swing(Time swing) {
  if (swing > get_swing())
    cur_swing = get_swing();
  else if (swing < 0_s)
    cur_swing = 0_s;
  else
    cur_swing = swing;
}

bool RepairTool::action_possible(const GoioObj* const obj) const {
  return !(((get_heal() > 0_hp && obj->get_health() == obj->get_max_health()) ||
              get_heal() == 0_hp) &&
           (get_extinguish() == 0 ||
              (get_extinguish() > 0 && obj->get_fire_stacks() <= 0)));
}

DmgState RepairTool::repair(GoioObj* obj, Time time) {
  cur_swing = get_swing();

  if (obj->dead()) {
    done = 2;
    std::cout << "                            ";
    return DmgState::NONE;
  }

  if (!action_possible(obj)) {
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
  if (obj->get_health() > 0_hp) {
    done = 0;

    obj->add_health(get_heal(), time + get_cooldown());
    if (get_heal() > 0_hp)
      ret |= DmgState::TARGET;
    auto immunity_end = time + get_fire_immune();
    if (get_fire_immune() > 0_s && immunity_end > obj->get_immunity_end()) {
      if (obj->get_immunity_end() <= time)
        ret |= DmgState::START_IMMUNITY;
      else
        ret |= DmgState::IMMUNITY;
    }

    obj->add_fire(-get_extinguish(), immunity_end);
    if (get_extinguish() > 0)
      ret |= DmgState::FIRE;

    auto start_buff = obj->get_buff_state() == 0;
    if (obj->add_buff(get_buff(), time + get_buff_duration(obj))) {
      ret |= DmgState::START_BUFF;
    } else if (get_buff() > 0) {
      if (start_buff)
        ret |= DmgState::START_PREBUFF;
      else
        ret |= DmgState::PREBUFF;
    }

    repair_wait = get_cooldown();
  } else {
    done = 1;
    auto start_rebuild = obj->get_rebuild_state() == 0;
    if (obj->add_rebuild(get_rebuild_power())) {
      ret |= DmgState::TRANSITIONED;
    } else if (get_rebuild_power() > 0) {
      if (start_rebuild)
        ret |= DmgState::START_REBUILD;
      else
        ret |= DmgState::REBUILD;
    }
    repair_wait = 0_s;
  }

  return ret;
}

void RepairTool::reset(bool) {
  done = 0;
  cur_swing = get_swing();
}

TimeFunc RepairTool::get_time_func(const GoioObj* obj, Time time, bool& force) {
  switch (done) {
    case 1:
      return std::bind(&RepairTool::get_cur_swing, this);
    case 2:
      if (!action_possible(obj))
        return nullptr;

      if (obj->get_health() == 0_hp) {
        if (obj->get_hull()->dead())
          return nullptr;
        cur_swing = Time(swing_foreshadowing_delay);
        force = true;
        return std::bind(&RepairTool::get_cur_swing, this);
      }

      return std::bind(&RepairTool::get_cooldown, this);
    case 0:
      if (obj->get_health() == 0_hp) {
        // if (get_rebuild_power() == 0) {
        //   done = 2;
        //   return nullptr;
        // }

        auto timediff = get_cur_swing() - time;
        // still in swing or ready? (tool usage ahead of time)
        if (timediff >= -0.00000001_s)  // take care of floating point errors
          cur_swing = timediff;
        else
          cur_swing = Time(swing_foreshadowing_delay);
        // std::cout << "\nset swing delay: "
        //           << cur_swing << " "
        //           << get_name() << std::endl
        //           << "                                    ";
        force = true;
        repair_wait = 0_s;
        return std::bind(&RepairTool::get_cur_swing, this);
      }
      if (get_swing() > get_cooldown())
        return std::bind(&RepairTool::get_cur_swing, this);
      else if (wait_cooldown() > 0_s)
        return std::bind(&RepairTool::wait_cooldown, this);
      else
        return std::bind(&RepairTool::get_cooldown, this);
    default:
      assert(false);
  }
}

}  // namespace goio
