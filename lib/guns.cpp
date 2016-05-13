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

void Gun::set_clipsize(int clipsize) {
  if (clipsize < 0)
    cur_clipsize = 0;
  else
    cur_clipsize = clipsize;
}
void Gun::set_rof(P_Time rof) {
  if (rof < 0/1_s)
    cur_rof = 0/1_s;
  else
    cur_rof = rof;
}
void Gun::set_reload(Time reload) {
  if (reload < 0_s)
    cur_reload = 0_s;
  else
    cur_reload = reload;
}
void Gun::set_direct_dmg(Health direct_dmg) {
  if (direct_dmg < 0_hp)
    cur_direct_dmg = 0_hp;
  else
    cur_direct_dmg = direct_dmg;
}
void Gun::set_direct_dmg_type(DmgType direct_dmg_type) {
  cur_direct_dmg_type = direct_dmg_type;
}
void Gun::set_aoe_dmg(Health aoe_dmg) {
  if (aoe_dmg < 0_hp)
    cur_aoe_dmg = 0_hp;
  else
    cur_aoe_dmg = aoe_dmg;
}
void Gun::set_aoe_dmg_type(DmgType aoe_dmg_type) {
  cur_aoe_dmg_type = aoe_dmg_type;
}
void Gun::set_aoe_radius(Distance aoe_radius) {
  if (aoe_radius < 0_m)
    cur_aoe_radius = 0_m;
  else
    cur_aoe_radius = aoe_radius;
}
void Gun::set_arming_time(Time arming_time) {
  if (arming_time < 0_s)
    cur_arming_time = 0_s;
  else
    cur_arming_time = arming_time;
}
void Gun::set_direct_ign_chance(double ign_chance) {
  if (ign_chance < 0)
    cur_direct_ign_chance = 0;
  else if (ign_chance > 1)
    cur_direct_ign_chance = 1;
  else
    cur_direct_ign_chance = ign_chance;
}
void Gun::set_aoe_ign_chance(double ign_chance) {
  if (ign_chance < 0)
    cur_aoe_ign_chance = 0;
  else if (ign_chance > 1)
    cur_aoe_ign_chance = 1;
  else
    cur_aoe_ign_chance = ign_chance;
}
void Gun::set_range(Distance range) {
  if (range < 0_m)
    cur_range = 0_m;
  else
    cur_range = range;
}
void Gun::set_projectile_speed(Speed projectile_speed) {
  if (projectile_speed < 0_m/1_s)
    cur_projectile_speed = 0_m/1_s;
  else
    cur_projectile_speed = projectile_speed;
}
void Gun::set_shell_drop(Acceleration shell_drop) {
  if (shell_drop < 0_m_s2)
    cur_shell_drop = 0_m_s2;
  else
    cur_shell_drop = shell_drop;
}
void Gun::set_jitter(Angle jitter) {
  if (jitter < 0_deg)
    cur_jitter = 0_deg;
  else
    cur_jitter = jitter;
}
void Gun::set_turn_left(Angle turn_left) {
  if (turn_left < 0_deg)
    cur_turn_left = 0_deg;
  else
    cur_turn_left = turn_left;
}
void Gun::set_turn_right(Angle turn_right) {
  if (turn_right < 0_deg)
    cur_turn_right = 0_deg;
  else
    cur_turn_right = turn_right;
}
void Gun::set_turn_up(Angle turn_up) {
  if (turn_up < 0_deg)
    cur_turn_up = 0_deg;
  else
    cur_turn_up = turn_up;
}
void Gun::set_turn_down(Angle turn_down) {
  if (turn_down < 0_deg)
    cur_turn_down = 0_deg;
  else
    cur_turn_down = turn_down;
}
void Gun::set_turn_horizontal(Angular_Speed turn_horizontal) {
  if (turn_horizontal < 0_deg/1_s)
    cur_turn_horizontal = 0_deg/1_s;
  else
    cur_turn_horizontal = turn_horizontal;
}
void Gun::set_turn_vertical(Angular_Speed turn_vertical) {
  if (turn_vertical < 0_deg/1_s)
    cur_turn_vertical = 0_deg/1_s;
  else
    cur_turn_vertical = turn_vertical;
}

int Gun::get_max_clipsize() const {
  if (get_ammo() != nullptr)
    return get_orig_clipsize() * get_ammo()->get_clipsize();
  return get_orig_clipsize();
}

bool Gun::apply_ammunition(const Ammunition* ammo) {
  if (ammo == nullptr)
    return false;
  set_clipsize(get_orig_clipsize() * ammo->get_clipsize());
  set_direct_dmg(get_orig_direct_dmg() * ammo->get_damage());
  set_aoe_dmg(get_orig_aoe_dmg() * ammo->get_damage());
  set_rof(get_orig_rof() * ammo->get_rof());
  set_aoe_radius(get_orig_aoe_radius() * ammo->get_aoe_radius());
  set_arming_time(get_orig_arming_time() * ammo->get_arming_time());
  set_direct_ign_chance(get_orig_direct_ign_chance() + ammo->get_ign_chance());
  set_aoe_ign_chance(get_orig_aoe_ign_chance() + ammo->get_ign_chance());
  set_projectile_speed(get_orig_projectile_speed() * ammo->get_projectile_speed());
  set_shell_drop(get_orig_shell_drop() * ammo->get_shell_drop());
  set_jitter(get_orig_jitter() * ammo->get_jitter());
  set_turn_horizontal(get_orig_turn_horizontal() * ammo->get_turn_speed());
  set_turn_vertical(get_orig_turn_vertical() * ammo->get_turn_speed());
  add_fire(ammo->get_fire_stacks());
  set_temporary_immunity(ammo->get_immune());

  cur_ammo = ammo;
  return true;
}

void Gun::reload(bool with_ammo) {
  set_reload(get_orig_reload());
  set_direct_dmg_type(get_orig_direct_dmg_type());
  set_aoe_dmg_type(get_orig_aoe_dmg_type());

  if (get_ammo() != nullptr && with_ammo) {
    apply_ammunition(get_ammo());
  } else {
    set_clipsize(get_orig_clipsize());
    set_direct_dmg(get_orig_direct_dmg());
    set_aoe_dmg(get_orig_aoe_dmg());
    set_rof(get_orig_rof());
    set_aoe_radius(get_orig_aoe_radius());
    set_arming_time(get_orig_arming_time());
    set_direct_ign_chance(get_orig_direct_ign_chance());
    set_aoe_ign_chance(get_orig_aoe_ign_chance());
    set_projectile_speed(get_orig_projectile_speed());
    set_shell_drop(get_orig_shell_drop());
    set_jitter(get_orig_jitter());
    set_turn_horizontal(get_orig_turn_horizontal());
    set_turn_vertical(get_orig_turn_vertical());
    cur_ammo = nullptr;
  }
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
    reload();
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
                          random_percentage() < get_direct_ign_chance()*100) {
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
      if (random_percentage() < ign_chance*100) {
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
                             random_percentage() < get_aoe_ign_chance()*100) {
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
        if (random_percentage() < ign_chance*100) {
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
