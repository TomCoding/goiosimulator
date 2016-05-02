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

#include "./ships.h"


namespace goio {

REGISTER_TYPE(Pyramidion, "Pyramidion");
REGISTER_TYPE(Goldfish, "Goldfish");
REGISTER_TYPE(Junker, "Junker");
REGISTER_TYPE(Galleon, "Galleon");
REGISTER_TYPE(Squid, "Squid");
REGISTER_TYPE(Spire, "Spire");
REGISTER_TYPE(Mobula, "Mobula");

inline Thrust Ship::get_thrust() const {
  return light_engines*light_engines_thrust +
         heavy_engines*heavy_engines_thrust;
}

inline Acceleration Ship::get_longitudinal_acceleration() const {
  return get_thrust()/get_mass();
}

double Ship::get_longitudinal_drag() const {
  return 2_m*get_longitudinal_acceleration() /
         (get_longitudinal_top_speed()*get_longitudinal_top_speed());
}

inline Acceleration Ship::get_vertical_acceleration() const {
  return get_lift_force()/get_mass();
}

double Ship::get_vertical_drag() const {
  return 2_m*get_vertical_acceleration() /
         (get_vertical_top_speed()*get_vertical_top_speed());
}

}  // namespace goio
