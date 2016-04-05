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

#include "./fire.h"

#include <iostream>

#include "./dmg_types.h"


namespace goio {

bool Fire::burn(GoioObj* obj, double, bool&) {
  if (obj->get_fire_stacks() < 1)
    return false;
  obj->add_health(-firetick *
                   dmg_types[DmgType::FIRE]
                            [obj->get_cmp_type()] *
                   (2*obj->get_fire_stacks()+8));
  return true;
}

TimeFunc Fire::get_time_func(const GoioObj* obj, double, bool&) {
  if (obj->get_fire_stacks() > 0)
    return &Fire::get_firetick;
  return nullptr;
}

}  // namespace goio
