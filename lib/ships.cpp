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
#include <cmath>

#include "./helmtools.h"


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
                   double longitudinal_drag,
                   Angular_Speed angular_top_speed,
                   double angular_drag,
                   Force lift_force,
                   double vertical_drag) :
                      mass(mass),
                      light_engines(light_engines),
                      light_engine_thrust(light_engine_thrust),
                      heavy_engines(heavy_engines),
                      heavy_engine_thrust(heavy_engine_thrust),
                      longitudinal_drag(longitudinal_drag),
                      angular_top_speed(angular_top_speed),
                      angular_drag(angular_drag),
                      lift_force(lift_force),
                      vertical_drag(vertical_drag) {
}

inline Thrust Shipinfo::get_thrust_int(Thrust light_engine_thrust,
                                       Thrust heavy_engine_thrust) const {
  return light_engines*light_engine_thrust +
         heavy_engines*heavy_engine_thrust;
}
inline Speed Shipinfo::get_top_speed_int(double drag,
                                         Acceleration acceleration) const {
  return sqrt(2_m*acceleration/drag);
}
inline Acceleration Shipinfo::get_acceleration_int(Force force,
                                                   Weight mass) const {
  return force/mass;
}
inline Angular_Acceleration Shipinfo::get_angular_acceleration_int(
                                          double drag,
                                          Angular_Speed top_speed) const {
  return top_speed*top_speed*drag/2_deg;
}

inline Thrust Shipinfo::get_orig_thrust() const {
  return get_thrust_int(get_orig_light_engine_thrust(),
                        get_orig_heavy_engine_thrust());
}
inline Speed Shipinfo::get_orig_longitudinal_top_speed() const {
  return get_top_speed_int(get_orig_longitudinal_drag(),
                           get_orig_longitudinal_acceleration());
}
inline Acceleration Shipinfo::get_orig_longitudinal_acceleration() const {
  return get_acceleration_int(get_orig_thrust(), get_orig_mass());
}
inline Angular_Acceleration Shipinfo::get_orig_angular_acceleration() const {
  return get_angular_acceleration_int(get_orig_angular_drag(),
                                      get_orig_angular_top_speed());
}
inline Speed Shipinfo::get_orig_vertical_top_speed() const {
  return get_top_speed_int(get_orig_vertical_drag(),
                           get_orig_vertical_acceleration());
}
inline Acceleration Shipinfo::get_orig_vertical_acceleration() const {
  return get_acceleration_int(get_orig_lift_force(), get_orig_mass());
}


Ship::Ship(const std::string& name,
           Health max_health, Health hull_max_health,
           Weight mass,
           int light_engines, Thrust light_engine_thrust,
           int heavy_engines, Thrust heavy_engine_thrust,
           double longitudinal_drag,
           Angular_Speed angular_top_speed,
           double angular_drag,
           Force lift_force,
           double vertical_drag) :
              Shipinfo(mass,
                       light_engines, light_engine_thrust,
                       heavy_engines, heavy_engine_thrust,
                       longitudinal_drag,
                       angular_top_speed, angular_drag,
                       lift_force, vertical_drag),
              GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health),
              cur_mass_mod(0),
              cur_light_engine_thrust_mod(0),
              cur_heavy_engine_thrust_mod(0),
              cur_longitudinal_drag_mod(0),
              cur_angular_top_speed_mod(0),
              cur_angular_drag_mod(0),
              cur_lift_force_mod(0),
              cur_descent_force_mod(0),
              cur_vertical_drag_mod(0),
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

void Ship::add_mass_mod(double mass) {
  cur_mass_mod += mass;
  if (cur_mass_mod < -1)
    cur_mass_mod = -1;
}
void Ship::add_light_engine_thrust_mod(double thrust) {
  cur_light_engine_thrust_mod += thrust;
  if (cur_light_engine_thrust_mod < -1)
    cur_light_engine_thrust_mod = -1;
}
void Ship::add_heavy_engine_thrust_mod(double thrust) {
  cur_heavy_engine_thrust_mod += thrust;
  if (cur_heavy_engine_thrust_mod < -1)
    cur_heavy_engine_thrust_mod = -1;
}
void Ship::add_longitudinal_drag_mod(double drag) {
  cur_longitudinal_drag_mod += drag;
  if (cur_longitudinal_drag_mod < -1)
    cur_longitudinal_drag_mod = -1;
}
void Ship::add_angular_top_speed_mod(double speed) {
  cur_angular_top_speed_mod += speed;
  if (cur_angular_top_speed_mod < -1)
    cur_angular_top_speed_mod = -1;
}
void Ship::add_angular_drag_mod(double drag) {
  cur_angular_drag_mod += drag;
  if (cur_angular_drag_mod < -1)
    cur_angular_drag_mod = -1;
}
void Ship::add_lift_force_mod(double lift_force) {
  cur_lift_force_mod += lift_force;
  if (cur_lift_force_mod < -1)
    cur_lift_force_mod = -1;
}
void Ship::add_descent_force_mod(double descent_force) {
  cur_descent_force_mod += descent_force;
  if (cur_descent_force_mod < -1)
    cur_descent_force_mod = -1;
}
void Ship::add_vertical_drag_mod(double drag) {
  cur_vertical_drag_mod += drag;
  if (cur_vertical_drag_mod < -1)
    cur_vertical_drag_mod = -1;
}

inline Thrust Ship::get_thrust() const {
  return get_thrust_int(get_light_engine_thrust(),
                        get_heavy_engine_thrust());
}
inline Speed Ship::get_longitudinal_top_speed() const {
  return get_top_speed_int(get_longitudinal_drag(),
                           get_longitudinal_acceleration());
}
inline Acceleration Ship::get_longitudinal_acceleration() const {
  return get_acceleration_int(get_thrust(), get_mass());
}
inline Angular_Acceleration Ship::get_angular_acceleration() const {
  return get_angular_acceleration_int(get_angular_drag(),
                                      get_angular_top_speed());
}
inline Speed Ship::get_lift_top_speed() const {
  return get_top_speed_int(get_vertical_drag(),
                           get_lift_acceleration());
}
inline Speed Ship::get_descent_top_speed() const {
  return get_top_speed_int(get_vertical_drag(),
                           get_descent_acceleration());
}
inline Acceleration Ship::get_lift_acceleration() const {
  return get_acceleration_int(get_lift_force(), get_mass());
}
inline Acceleration Ship::get_descent_acceleration() const {
  return get_acceleration_int(get_descent_force(), get_mass());
}

void Ship::apply_tool(const HelmTool* tool) {
  cur_mass_mod = 0;
  cur_light_engine_thrust_mod = 0;
  cur_heavy_engine_thrust_mod = 0;
  cur_longitudinal_drag_mod = 0;
  cur_angular_top_speed_mod = 0;
  cur_angular_drag_mod = 0;
  cur_lift_force_mod = 0;
  cur_descent_force_mod = 0;
  cur_vertical_drag_mod = 0;

  // apply buff and other modifiers here

  if (tool != nullptr) {
    add_light_engine_thrust_mod(tool->get_thrust());
    add_heavy_engine_thrust_mod(tool->get_thrust());
    add_angular_drag_mod(tool->get_angular_drag());
    add_longitudinal_drag_mod(tool->get_longitudinal_drag());
    add_lift_force_mod(tool->get_lift_force());
    add_descent_force_mod(tool->get_descent_force());
    add_vertical_drag_mod(tool->get_vertical_drag());
  }

  for (auto it = engines_l.begin(); it != engines_l.end(); ++it) {
    (*it)->reset_modifiers();
    (*it)->add_thrust_mod(tool->get_thrust());
  }
  for (auto it = engines_h.begin(); it != engines_h.end(); ++it) {
    (*it)->reset_modifiers();
    (*it)->add_thrust_mod(tool->get_thrust());
  }

  balloon->reset_modifiers();
  balloon->add_lift_force_mod(tool->get_lift_force());
  balloon->add_descent_force_mod(tool->get_descent_force());

  cur_tool = tool;
}

void Ship::add_gun(Gun* gun) {
  gun->set_hull(this);
  guns.insert(gun);
}

}  // namespace goio
