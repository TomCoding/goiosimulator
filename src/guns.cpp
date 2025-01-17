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

REGISTER_TYPE(Artemis, "Artemis");
REGISTER_TYPE(Banshee, "Banshee");
REGISTER_TYPE(LightCaro, "LightCaro");
REGISTER_TYPE(Flare, "Flare");
REGISTER_TYPE(Flamethrower, "Flamethrower");
REGISTER_TYPE(Gatling, "Gatling");

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

int Gun::get_max_clipsize() const {
  if (get_ammo() != nullptr)
    return get_orig_clipsize() * get_ammo()->get_clipsize();
  return get_orig_clipsize();
}

P_Time Gun::get_max_rof() const {
  if (get_ammo() != nullptr)
    return get_orig_rof() * get_ammo()->get_rof();
  return get_orig_rof();
}

bool Gun::apply_ammunition(const Ammunition* ammo) {
  if (ammo == nullptr)
    return false;
  set_clipsize(get_orig_clipsize() * ammo->get_clipsize());
  set_direct_dmg(get_orig_direct_dmg() * ammo->get_damage());
  set_aoe_dmg(get_orig_aoe_dmg() * ammo->get_damage());
  set_rof(get_orig_rof() * ammo->get_rof());
  set_aoe_radius(get_orig_aoe_radius() * ammo->get_aoe_radius());
  set_direct_ign_chance(get_orig_direct_ign_chance() + ammo->get_ign_chance());
  set_aoe_ign_chance(get_orig_aoe_ign_chance() + ammo->get_ign_chance());

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
    set_direct_ign_chance(get_orig_direct_ign_chance());
    set_aoe_ign_chance(get_orig_aoe_ign_chance());
    cur_ammo = nullptr;
  }
}

inline P_Time Gun::get_rof_changed() const {
  return get_health()/get_max_health()*get_max_rof();
}

inline Time Gun::get_time_per_shot() const {
  return get_max_health()/get_health()/get_max_rof();
}

inline Time Gun::get_reload_changed() const {
  return get_max_health()/get_health()*get_reload();
}

DmgState::State Gun::shoot(GoioObj* obj, Time time, bool aoe, Distance aoe_range) {
  if (during_reload) {
    reload();
    during_reload = false;
  }
  if (get_health() == 0_hp || get_clipsize() == 0 ||
          get_fire_stacks() >= GoioObj::get_fire_stacks_unusable()) {
    std::cout << "                            ";
    return DmgState::NONE;
  }

  GoioObj* tmpobj;
  if (obj->get_health() > 0_hp) {
    tmpobj = obj;
  } else {
    tmpobj = obj->get_hull();
    if (tmpobj->get_health() == 0_hp) {
      std::cout << "                            ";
      return DmgState::NONE;
    }
  }

  dec_clipsize();
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

  if (aoe && aoe_range <= get_aoe_radius()) {
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
