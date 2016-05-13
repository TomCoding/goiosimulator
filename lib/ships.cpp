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

REGISTER_TYPE(Pyramidion);
REGISTER_TYPE(Goldfish);
REGISTER_TYPE(Junker);
REGISTER_TYPE(Galleon);
REGISTER_TYPE(Squid);
REGISTER_TYPE(Spire);
REGISTER_TYPE(Mobula);

Shipinfo::Shipinfo(Weight mass,
                   int light_engines, Thrust light_engine_thrust,
                   int heavy_engines, Thrust heavy_engine_thrust,
                   Speed longitudinal_top_speed,
                   Angular_Speed angular_top_speed,
                   Angular_Acceleration angular_acceleration,
                   Force lift_force,
                   Speed vertical_top_speed) :
                      mass(mass),
                      light_engines(light_engines),
                      light_engine_thrust(light_engine_thrust),
                      heavy_engines(heavy_engines),
                      heavy_engine_thrust(heavy_engine_thrust),
                      longitudinal_top_speed(longitudinal_top_speed),
                      angular_top_speed(angular_top_speed),
                      angular_acceleration(angular_acceleration),
                      lift_force(lift_force),
                      vertical_top_speed(vertical_top_speed) {
}

inline Thrust Shipinfo::get_thrust_int(Thrust light_engine_thrust,
                                       Thrust heavy_engine_thrust) const {
  return light_engines*light_engine_thrust +
         heavy_engines*heavy_engine_thrust;
}
inline Acceleration Shipinfo::get_longitudinal_acceleration_int(
                                       Thrust thrust,
                                       Weight mass) const {
  return thrust/mass;
}
inline double Shipinfo::get_longitudinal_drag_int(
                                       Acceleration longitudinal_acceleration,
                                       Speed longitudinal_top_speed) const {
  return 2_m*longitudinal_acceleration /
         (longitudinal_top_speed*longitudinal_top_speed);
}
inline Acceleration Shipinfo::get_vertical_acceleration_int(
                                       Force lift_force,
                                       Weight mass) const {
  return lift_force/mass;
}
inline double Shipinfo::get_vertical_drag_int(
                                       Acceleration vertical_acceleration,
                                       Speed vertical_top_speed) const {
  return 2_m*vertical_acceleration /
         (vertical_top_speed*vertical_top_speed);
}

inline Thrust Shipinfo::get_orig_thrust() const {
  return get_thrust_int(get_orig_light_engine_thrust(),
                        get_orig_heavy_engine_thrust());
}
inline Acceleration Shipinfo::get_orig_longitudinal_acceleration() const {
  return get_longitudinal_acceleration_int(get_orig_thrust(), get_orig_mass());
}
inline double Shipinfo::get_orig_longitudinal_drag() const {
  return get_longitudinal_drag_int(get_orig_longitudinal_acceleration(),
                                   get_orig_longitudinal_top_speed());
}
inline Acceleration Shipinfo::get_orig_vertical_acceleration() const {
  return get_vertical_acceleration_int(get_orig_lift_force(), get_orig_mass());
}
inline double Shipinfo::get_orig_vertical_drag() const {
  return get_vertical_drag_int(get_orig_vertical_acceleration(),
                               get_orig_vertical_top_speed());
}


Ship::Ship(const std::string& name, Health max_health, Health hull_max_health,
           Weight mass, int light_engines, Thrust light_engine_thrust,
           int heavy_engines, Thrust heavy_engine_thrust,
           Speed longitudinal_top_speed, Angular_Speed angular_top_speed,
           Angular_Acceleration angular_acceleration, Force lift_force,
           Speed vertical_top_speed) :
              Shipinfo(mass,
                       light_engines, light_engine_thrust,
                       heavy_engines, heavy_engine_thrust,
                       longitudinal_top_speed,
                       angular_top_speed, angular_acceleration,
                       lift_force, vertical_top_speed),
              GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health),
              cur_mass(mass),
              cur_light_engine_thrust(light_engine_thrust),
              cur_heavy_engine_thrust(heavy_engine_thrust),
              cur_longitudinal_top_speed(longitudinal_top_speed),
              cur_angular_top_speed(angular_top_speed),
              cur_angular_acceleration(angular_acceleration),
              cur_lift_force(lift_force),
              cur_descent_force(lift_force),
              cur_vertical_top_speed(vertical_top_speed),
              balloon(new Balloon("", lift_force)),
              engines_l(), engines_h(), guns(), cur_tool(nullptr) {
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

void Ship::set_mass(Weight mass) {
  if (mass <= 0_kg)
    cur_mass = 1_kg;
  else
    cur_mass = 1_kg;
}
void Ship::set_light_engine_thrust(Thrust thrust) {
  if (thrust < 0_N)
    cur_light_engine_thrust = 0_N;
  else
    cur_light_engine_thrust = thrust;
}
void Ship::set_heavy_engine_thrust(Thrust thrust) {
  if (thrust < 0_N)
    cur_heavy_engine_thrust = 0_N;
  else
    cur_heavy_engine_thrust = thrust;
}
void Ship::set_longitudinal_top_speed(Speed speed) {
  if (speed < 0_m/1_s)
    cur_longitudinal_top_speed = 0_m/1_s;
  else
    cur_longitudinal_top_speed = speed;
}
void Ship::set_angular_top_speed(Angular_Speed speed) {
  if (speed < 0_deg/1_s)
    cur_angular_top_speed = 0_deg/1_s;
  else
    cur_angular_top_speed = speed;
}
void Ship::set_angular_acceleration(Angular_Acceleration acceleration) {
  if (acceleration < 0_deg_s2)
    cur_angular_acceleration = 0_deg_s2;
  else
    cur_angular_acceleration = acceleration;
}
void Ship::set_lift_force(Force lift_force) {
  if (lift_force < 0_N)
    cur_lift_force = 0_N;
  else
    cur_lift_force = lift_force;
}
void Ship::set_descent_force(Force descent_force) {
  if (descent_force < 0_N)
    cur_descent_force = 0_N;
  else
    cur_descent_force = descent_force;
}
void Ship::set_vertical_top_speed(Speed speed) {
  if (speed < 0_m/1_s)
    cur_vertical_top_speed = 0_m/1_s;
  else
    cur_vertical_top_speed = speed;
}

inline Thrust Ship::get_thrust() const {
  return get_thrust_int(get_light_engine_thrust(),
                        get_heavy_engine_thrust());
}
inline Acceleration Ship::get_longitudinal_acceleration() const {
  return get_longitudinal_acceleration_int(get_thrust(), get_mass());
}
inline double Ship::get_longitudinal_drag() const {
  return get_longitudinal_drag_int(get_longitudinal_acceleration(),
                                   get_longitudinal_top_speed());
}
inline Acceleration Ship::get_lift_acceleration() const {
  return get_vertical_acceleration_int(get_lift_force(), get_mass());
}
inline Acceleration Ship::get_descent_acceleration() const {
  return get_vertical_acceleration_int(get_descent_force(), get_mass());
}
inline double Ship::get_lift_drag() const {
  return get_vertical_drag_int(get_lift_acceleration(),
                               get_vertical_top_speed());
}
inline double Ship::get_descent_drag() const {
  return get_vertical_drag_int(get_descent_acceleration(),
                               get_vertical_top_speed());
}

void Ship::add_gun(Gun* gun) {
  gun->set_hull(this);
}

void Ship::reset(bool) {
  cur_mass = get_orig_mass();
  cur_light_engine_thrust = get_orig_light_engine_thrust();
  cur_heavy_engine_thrust = get_orig_heavy_engine_thrust();
  cur_longitudinal_top_speed = get_orig_longitudinal_top_speed();
  cur_angular_top_speed = get_orig_angular_top_speed();
  cur_angular_acceleration = get_orig_angular_acceleration();
  cur_lift_force = get_orig_lift_force();
  cur_descent_force = get_orig_lift_force();
  cur_vertical_top_speed = get_orig_vertical_top_speed();
}

}  // namespace goio
