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

#include <string>
#include <set>


namespace goio {

REGISTER_TYPE(Pyramidion, "Pyramidion");
REGISTER_TYPE(Goldfish, "Goldfish");
REGISTER_TYPE(Junker, "Junker");
REGISTER_TYPE(Galleon, "Galleon");
REGISTER_TYPE(Squid, "Squid");
REGISTER_TYPE(Spire, "Spire");
REGISTER_TYPE(Mobula, "Mobula");

Ship::Ship(const std::string& name, Health max_health, Health hull_max_health,
           Weight mass, int light_engines, Thrust light_engine_thrust,
           int heavy_engines, Thrust heavy_engine_thrust,
           Speed longitudinal_top_speed, Angular_Speed angular_top_speed,
           Angular_Acceleration angular_acceleration, Force lift_force,
           Speed vertical_top_speed) :
              GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health),
              mass(mass),
              light_engines(light_engines),
              light_engine_thrust(light_engine_thrust),
              heavy_engines(heavy_engines),
              heavy_engine_thrust(heavy_engine_thrust),
              longitudinal_top_speed(longitudinal_top_speed),
              angular_top_speed(angular_top_speed),
              angular_acceleration(angular_acceleration),
              lift_force(lift_force),
              vertical_top_speed(vertical_top_speed),
              balloon(new Balloon("", lift_force)),
              engines_l(), engines_h(), guns() {

  balloon->set_hull(this);
  for (int i = 0; i < light_engines; ++i) {
    auto engine = new LightEngine("", light_engine_thrust);
    engine->set_hull(this);
    engines_l.insert(engine);
  }
  for (int i = 0; i < heavy_engines; ++i) {
    auto engine = new HeavyEngine("", heavy_engine_thrust);
    engine->set_hull(this);
    engines_h.insert(engine);
  }
}

Ship::~Ship() {
  for (auto it = engines_l.begin(); it != engines_l.end(); ++it)
    delete *it;
  for (auto it = engines_h.begin(); it != engines_h.end(); ++it)
    delete *it;
  delete balloon;
}

inline Thrust Ship::get_thrust() const {
  return light_engines*light_engine_thrust +
         heavy_engines*heavy_engine_thrust;
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

void Ship::add_gun(Gun* gun) {
  gun->set_hull(this);
}

}  // namespace goio
