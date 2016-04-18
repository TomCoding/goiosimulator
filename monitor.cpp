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

#include "./monitor.h"

#include <iostream>

#include "./dmg_types.h"


namespace goio {

DmgState::State Monitor::monitor(GoioObj*, Second) {
  std::cout << "                            ";
  return DmgState::NONE;
}

TimeFunc Monitor::get_time_func(const GoioObj* obj, Second, bool&) {
  if (obj->get_health() == 0_hp && obj->get_hull()->get_health() == 0_hp)
    return nullptr;
  return std::bind(&Monitor::get_tick, this);
}

}  // namespace goio
