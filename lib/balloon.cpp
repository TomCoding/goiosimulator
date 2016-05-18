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

#include "./balloon.h"

#include <string>


namespace goio {

Balloon::Balloon(const std::string& name, Force lift_force, Health max_health) :
                 BalloonInfo(lift_force),
                 GoioObj(name, CmpType::BALLOON, 0.666666, max_health),
                 cur_lift_force_mod(0),
                 cur_descent_force_mod(0) {
  if (max_health <= 0_hp)
    throw NonPositiveHealth(max_health);
}

void Balloon::add_lift_force_mod(double lift_force) {
  cur_lift_force_mod += lift_force;
  if (cur_lift_force_mod < -1)
    cur_lift_force_mod = -1;
}
void Balloon::add_descent_force_mod(double descent_force) {
  cur_descent_force_mod += descent_force;
  if (cur_descent_force_mod < -1)
    cur_descent_force_mod = -1;
}

Force Balloon::get_lift_force_changed() const {
  return (get_health()/get_max_health())*get_lift_force();
}
Force Balloon::get_descent_force_changed() const {
  return (get_health()/get_max_health())*get_descent_force();
}

void Balloon::reset_modifiers() {
  cur_lift_force_mod = 0;
  cur_descent_force_mod = 0;
}

}  // namespace goio
