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

#include "./engines.h"


namespace goio {

void Engine::add_thrust_mod(double thrust) {
  cur_thrust_mod += thrust;
  if (cur_thrust_mod < -1)
    cur_thrust_mod = -1;
}

Thrust Engine::get_thrust_changed() const {
  return (get_health()/get_max_health())*get_thrust();
}

void Engine::reset_modifiers() {
  cur_thrust_mod = 0;
}

}  // namespace goio
