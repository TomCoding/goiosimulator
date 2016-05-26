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

#include "./guns.h"

#include <cmath>
#include <functional>

#include <iostream>
#include <iomanip>

#include "./dmg_types.h"
#include "./rng.h"


namespace goio {

REGISTER_TYPE(Artemis);
REGISTER_TYPE(Banshee);
REGISTER_TYPE(LightCaro);
REGISTER_TYPE(Flare);
REGISTER_TYPE(Flamethrower);
REGISTER_TYPE(LightFlak);
REGISTER_TYPE(Hades);
REGISTER_TYPE(Harpoon);
REGISTER_TYPE(Mercury);
REGISTER_TYPE(MineLauncher);
REGISTER_TYPE(Mortar);
REGISTER_TYPE(Gatling);

REGISTER_TYPE(HeavyCaro);
REGISTER_TYPE(Hwacha);
REGISTER_TYPE(Lumberjack);
REGISTER_TYPE(HeavyFlak);
REGISTER_TYPE(Minotaur);

void Gun::add_clipsize_mod(double clipsize) {
  cur_clipsize_mod += clipsize;
  if (cur_clipsize_mod < -1)
    cur_clipsize_mod = -1;
}
void Gun::add_rof_mod(double rof) {
  cur_rof_mod += rof;
  if (cur_rof_mod < -1)
    cur_rof_mod = -1;
}
void Gun::add_reload_mod(double reload) {
  cur_reload_mod += reload;
  if (cur_reload_mod < -1)
    cur_reload_mod = -1;
}
void Gun::add_direct_dmg_mod(double direct_dmg) {
  cur_direct_dmg_mod += direct_dmg;
  if (cur_direct_dmg_mod < -1)
    cur_direct_dmg_mod = -1;
}
void Gun::add_aoe_dmg_mod(double aoe_dmg) {
  cur_aoe_dmg_mod += aoe_dmg;
  if (cur_aoe_dmg_mod < -1)
    cur_aoe_dmg_mod = -1;
}
void Gun::add_aoe_radius_mod(double aoe_radius) {
  cur_aoe_radius_mod += aoe_radius;
  if (cur_aoe_radius_mod < -1)
    cur_aoe_radius_mod = -1;
}
void Gun::add_arming_time_mod(double arming_time) {
  cur_arming_time_mod += arming_time;
  if (cur_arming_time_mod < -1)
    cur_arming_time_mod = -1;
}
void Gun::add_direct_ign_chance_mod(double ign_chance) {
  cur_direct_ign_chance_mod += ign_chance;
  if (cur_direct_ign_chance_mod < -1)
    cur_direct_ign_chance_mod = -1;
}
void Gun::add_aoe_ign_chance_mod(double ign_chance) {
  cur_aoe_ign_chance_mod += ign_chance;
  if (cur_aoe_ign_chance_mod < -1)
    cur_aoe_ign_chance_mod = -1;
}
void Gun::add_range_mod(double range) {
  cur_range_mod += range;
  if (cur_range_mod < -1)
    cur_range_mod = -1;
}
void Gun::add_projectile_speed_mod(double projectile_speed) {
  cur_projectile_speed_mod += projectile_speed;
  if (cur_projectile_speed_mod < -1)
    cur_projectile_speed_mod = -1;
}
void Gun::add_shell_drop_mod(double shell_drop) {
  cur_shell_drop_mod += shell_drop;
  if (cur_shell_drop_mod < -1)
    cur_shell_drop_mod = -1;
}
void Gun::add_jitter_mod(double jitter) {
  cur_jitter_mod += jitter;
  if (cur_jitter_mod < -1)
    cur_jitter_mod = -1;
}
void Gun::add_turn_left_mod(double turn_left) {
  cur_turn_left_mod += turn_left;
  if (cur_turn_left_mod < -1)
    cur_turn_left_mod = -1;
}
void Gun::add_turn_right_mod(double turn_right) {
  cur_turn_right_mod += turn_right;
  if (cur_turn_right_mod < -1)
    cur_turn_right_mod = -1;
}
void Gun::add_turn_up_mod(double turn_up) {
  cur_turn_up_mod += turn_up;
  if (cur_turn_up_mod < -1)
    cur_turn_up_mod = -1;
}
void Gun::add_turn_down_mod(double turn_down) {
  cur_turn_down_mod += turn_down;
  if (cur_turn_down_mod < -1)
    cur_turn_down_mod = -1;
}
void Gun::add_turn_horizontal_mod(double turn_horizontal) {
  cur_turn_horizontal_mod += turn_horizontal;
  if (cur_turn_horizontal_mod < -1)
    cur_turn_horizontal_mod = -1;
}
void Gun::add_turn_vertical_mod(double turn_vertical) {
  cur_turn_vertical_mod += turn_vertical;
  if (cur_turn_vertical_mod < -1)
    cur_turn_vertical_mod = -1;
}

inline void Gun::set_direct_dmg_type(DmgType direct_dmg_type) {
  cur_direct_dmg_type = direct_dmg_type;
}
inline void Gun::set_aoe_dmg_type(DmgType aoe_dmg_type) {
  cur_aoe_dmg_type = aoe_dmg_type;
}

void Gun::reload(const Ammunition* ammo) {
  cur_direct_dmg_type = get_orig_direct_dmg_type();
  cur_aoe_dmg_type = get_orig_aoe_dmg_type();
  if (ammo != nullptr)
    apply_tool(ammo);
  else
    reset_modifiers();
  cur_ammo = ammo;
  cur_clipsize = get_max_clipsize();
}

void Gun::reset_modifiers() {
  cur_clipsize_mod = 0;
  cur_rof_mod = 0;
  cur_reload_mod = 0;
  cur_direct_dmg_mod = 0;
  cur_aoe_dmg_mod = 0;
  cur_aoe_radius_mod = 0;
  cur_arming_time_mod = 0;
  cur_direct_ign_chance_mod = 0;
  cur_aoe_ign_chance_mod = 0;
  cur_range_mod = 0;
  cur_projectile_speed_mod = 0;
  cur_shell_drop_mod = 0;
  cur_jitter_mod = 0;
  cur_turn_left_mod = 0;
  cur_turn_right_mod = 0;
  cur_turn_up_mod = 0;
  cur_turn_down_mod = 0;
  cur_turn_horizontal_mod = 0;
  cur_turn_vertical_mod = 0;
}

inline P_Time Gun::get_rof_changed() const {
  return get_health()/get_max_health()*get_rof();
}
inline Time Gun::get_time_per_shot() const {
  return get_max_health()/get_health()/get_rof();
}
inline Time Gun::get_reload_changed() const {
  return get_max_health()/get_health()*get_reload();
}
inline Angular_Speed Gun::get_turn_horizontal_changed() const {
  return get_max_health()/get_health()*get_turn_horizontal();
}
inline Angular_Speed Gun::get_turn_vertical_changed() const {
  return get_max_health()/get_health()*get_turn_vertical();
}

DmgState Gun::shoot(GoioObj* obj, Time time,
                           bool armed, Distance aoe_range) {
  if (during_reload) {
    reload(get_ammo());
    during_reload = false;
  }
  if (get_health() == 0_hp || get_clipsize() == 0 ||
          get_fire_stacks() >= GoioObj::get_fire_stacks_unusable()) {
    std::cout << "                            ";
    return DmgState::NONE;
  }

  GoioObj* tmpobj = obj;
  while (tmpobj->get_health() == 0_hp) {
    if (tmpobj->get_hull() != nullptr) {
      tmpobj = tmpobj->get_hull();
    } else {
      std::cout << "                            ";
      return DmgState::NONE;
    }
  }

  if (dec_clipsize() == 0)
    set_temporary_immunity(false);

  auto ret = DmgState::TARGET;
  if (obj->get_health() == obj->get_max_health())
    ret |= DmgState::START_TARGET;

  if (!tmpobj->add_health(-get_direct_dmg() *
                          get_dmg_modifier(get_direct_dmg_type(),
                          tmpobj->get_cmp_type())))

    ret |= DmgState::TRANSITIONED;

  auto start_fire = obj->get_fire_stacks() == 0;
  if (tmpobj->get_immunity_end() <= time) {
    if (get_direct_ign_chance() > 0 &&
                          random_percentage() < get_direct_ign_chance()) {
      if (tmpobj->add_fire(get_orig_direct_ign_stacks())) {
        if (start_fire)
          ret |= DmgState::START_FIRE;
        else
          ret |= DmgState::FIRE;
      }
    } else if (get_direct_dmg_type() == DmgType::EXPLOSIVE) {
      auto ign_chance = get_direct_dmg()/1_hp *
                        get_dmg_modifier(DmgType::EXPLOSIVE,
                                         tmpobj->get_cmp_type()) *
                        explosive_ign_chance;
      if (random_percentage() < ign_chance) {
        if (tmpobj->add_fire(explosive_stacks)) {
          if (start_fire)
            ret |= DmgState::START_FIRE;
          else
            ret |= DmgState::FIRE;
        }
      }
    }
  }

  if (armed && aoe_range <= get_aoe_radius()) {
    double falloff;
    if (aoe_range*aoe_radius_dmg_falloff <= get_aoe_radius())
      falloff = 1;
    else
      // falloff = 1.8-1.6*(aoe_range/get_aoe_radius());
      // 0.2 + 0.8*(r-l/(r/2)
      falloff = aoe_dmg_falloff_min +
               (1-aoe_dmg_falloff_min) *
               (get_aoe_radius()-aoe_range) /
               (get_aoe_radius()/aoe_radius_dmg_falloff);
    if (!tmpobj->add_health(-falloff*get_aoe_dmg() *
                            get_dmg_modifier(get_aoe_dmg_type(),
                            tmpobj->get_cmp_type())))
      ret |= DmgState::TRANSITIONED;
    if (tmpobj->get_immunity_end() <= time) {
      if (get_aoe_ign_chance() > 0 &&
                             random_percentage() < get_aoe_ign_chance()) {
        if (tmpobj->add_fire(get_orig_aoe_ign_stacks())) {
          if (start_fire)
            ret |= DmgState::START_FIRE;
          else
            ret |= DmgState::FIRE;
        }
      } else if (get_aoe_dmg_type() == DmgType::EXPLOSIVE) {
        auto ign_chance = get_aoe_dmg()/1_hp *
                          get_dmg_modifier(DmgType::EXPLOSIVE,
                                           tmpobj->get_cmp_type()) *
                          explosive_ign_chance;
        if (random_percentage() < ign_chance) {
          if (tmpobj->add_fire(explosive_stacks)) {
            if (start_fire)
              ret |= DmgState::START_FIRE;
            else
              ret |= DmgState::FIRE;
          }
        }
      }
    }
  }

  if (get_ammo() != nullptr && get_ammo()->get_proportional_self_damage()) {
    if (get_health() == get_max_health())
      ret |= DmgState::START_SELF;
    else
      ret |= DmgState::SELF;
    if (!add_health(-get_max_health()/get_max_clipsize()))
      ret |= DmgState::TRANSITIONED_S;
  }

  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;
  using std::setw;
  using std::right;
  cout << fixed << setprecision(2);
  cout << setw(15) << right << get_name();
  cout << setw(5) << get_clipsize();
  cout << setw(8) << get_health();

  return ret;
}

TimeFunc Gun::get_time_func(const GoioObj* obj, Time, bool&) {
  if (get_health() == 0_hp ||
        get_fire_stacks() >= GoioObj::get_fire_stacks_unusable() ||
        (obj->get_health() == 0_hp && obj->get_hull()->get_health() == 0_hp))
    return nullptr;
  // if (during_reload)
  //   return std::bind(&Gun::get_reload_changed, this);
  if (get_clipsize() > 0)
    return std::bind(&Gun::get_time_per_shot, this);
  during_reload = true;
  return std::bind(&Gun::get_reload_changed, this);
}

}  // namespace goio
