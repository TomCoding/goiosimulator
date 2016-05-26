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

#include "./goioobj.h"

#include <iostream>

#include "./dmg_types.h"
#include "./tooldispatcher.h"


namespace goio {

REGISTER_TYPE_FULL(FreeObject);

GoioObj::GoioObj(const std::string& name,
                 CmpType cmp_type,
                 double part_type_multiplier,
                 Health max_health,
                 Health hull_max_health) :
                    name(name), cmp_type(cmp_type),
                    max_health(max_health), health(max_health),
                    fire_stacks(-1), rebuild_state(-1), buff_state(0),
                    part_type_multiplier(part_type_multiplier),
                    hull(new Hull(hull_max_health)),
                    cooldown_end(0_s), immunity_end(0_s), buff_end(0_s),
                    temporary_immunity(false),
                    connected(false),
                    active_tools() {
}

GoioObj::~GoioObj() {
  if (!connected)
    delete hull;
}

bool GoioObj::add_health(Health health, Time cooldown_end) {
  if ((health > 0_hp && cmp_type == CmpType::HULL))
    return false;
  if (cooldown_end >= 0_s) {
    this->cooldown_end = cooldown_end;
    // std::cout << "\ncooldownS: " << get_name() << " "
    //           << cooldown_end << std::endl
    //           << "                                    ";
  }
  if (health == 0_hp)
    return true;
  return set_health_int(this->health + health, this);
}

bool GoioObj::add_fire(int fire, Time immunity_end, Time cooldown_end) {
  if (health == 0_hp || cmp_type == CmpType::HULL)
    return false;
  if (immunity_end >= 0_s)
    this->immunity_end = immunity_end;
  if (cooldown_end >= 0_s)
    this->cooldown_end = cooldown_end;

  if (fire_stacks == -1 || get_temporary_immunity())
    return true;

  auto new_fire = fire_stacks + fire;
  if (new_fire > max_fire_stacks)
    fire_stacks = max_fire_stacks;
  else if (new_fire <= 0)
    fire_stacks = 0;
  else
    fire_stacks = new_fire;
  return true;
}

bool GoioObj::add_rebuild(int rebuild_progress) {
  // if (cmp_type == CmpType::HULL)
  //   return false;
  rebuild_state += rebuild_progress;
  if (rebuild_state >= get_rebuild_value()) {
    health = max_health*health_after_rebuild;
    return true;
  }
  return false;
}

bool GoioObj::add_buff(int buff_progress, Time buff_end) {
  if (health == 0_hp || cmp_type == CmpType::HULL)
    return false;
  buff_state += buff_progress;
  if (buff_state >= get_buff_value()) {
    buff_state = 0;
    if (buff_end >= 0_s)
      this->buff_end = buff_end;
    return true;
  }
  return false;
}

bool GoioObj::set_health_int(Health health, GoioObj* obj) {
  if (obj->health == -1_hp)
    return true;
  if (health > obj->max_health) {
    obj->health = obj->max_health;
  } else if (health <= 0_hp) {
    obj->health = 0_hp;
    if (obj->fire_stacks > 0)
      obj->fire_stacks = 0;
    obj->buff_state = 0;
    if (obj->cmp_type != CmpType::HULL) {
      obj->rebuild_state = 0;
      obj->cooldown_end = 0_s;
      obj->buff_end = 0_s;
      // std::cout << "\ncooldown0: 0" << std::endl
      //           << "        ";
    }
    return false;
  } else {
    obj->health = health;
  }
  return true;
}

void GoioObj::set_health(Health health) {
  if (health > max_health)
    max_health = health;
  cooldown_end = 0_s;
  buff_end = 0_s;
  set_health_int(health, this);
}

void GoioObj::set_hull_health(Health health) {
  if (health == -1_hp) {
    hull->max_health = hull->health = -1_hp;
    return;
  }
  if (health > hull->max_health)
    hull->max_health = health;
  set_health_int(health, hull);
}

void GoioObj::set_fire(int fire) {
  if (fire > max_fire_stacks)
    fire_stacks = max_fire_stacks;
  else if (fire <= 0)
    fire_stacks = 0;
  else
    fire_stacks = fire;
  immunity_end = 0_s;
}

void GoioObj::set_buff(int buff) {
  if (buff > get_buff_value())
    buff_state = get_buff_value()-1;
  else if (buff <= 0)
    buff_state = 0;
  else
    buff_state = buff;
  buff_end = 0_s;
}

void GoioObj::set_hull(GoioObj* obj) {
  connected = true;
  delete hull;
  hull = obj;
}

bool GoioObj::dead() const {
  auto tmpobj = this;
  do {
    if (tmpobj->get_health() != 0_hp)
      return false;
  } while ((tmpobj = tmpobj->get_hull()) != nullptr);
  return true;
}

void GoioObj::reset(bool hull) {
  health = max_health;
  if (fire_stacks > 0)
    fire_stacks = 0;
  buff_state = 0;
  cooldown_end = 0_s;
  immunity_end = 0_s;
  buff_end = 0_s;
  if (hull && this->hull != nullptr)
    this->hull->reset(true);
}

void GoioObj::apply_tool(const Tool* tool) {
  if (tool == nullptr)
    return;

  auto it = active_tools.find(tool);
  if (it == active_tools.end()) {
    active_tools.insert(tool);
    ToolDispatcher td;
    accept(td, tool, true);
  }
}

void GoioObj::remove_tool(const Tool* tool) {
  if (tool == nullptr)
    return;

  auto it = active_tools.find(tool);
  if (it != active_tools.end()) {
    ToolDispatcher td;
    accept(td, tool, false);
    active_tools.erase(it);
  }
}

}  // namespace goio
