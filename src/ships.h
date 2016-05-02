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

#ifndef SHIPS_H_
#define SHIPS_H_

#include <string>

#include "./goioobj.h"


namespace goio {

class Ship : public GoioObj {
 private:
    const Weight mass;
    const int    light_engines;
    const Thrust light_engines_thrust;
    const int    heavy_engines;
    const Thrust heavy_engines_thrust;
    const Speed  longitudinal_top_speed;
    const Angular_Speed angular_top_speed;
    const Angular_Acceleration angular_acceleration;
    const Force  lift_force;
    const Speed  vertical_top_speed;

 protected:
    Ship(const std::string& name, Health max_health, Health hull_max_health,
         Weight mass,
         int light_engines, Thrust light_engines_thrust,
         int heavy_engines, Thrust heavy_engines_thrust,
         Speed longitudinal_top_speed,
         Angular_Speed angular_top_speed, Angular_Acceleration angular_acceleration,
         Force lift_force, Speed vertical_top_speed) :
              GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health),
              mass(mass),
              light_engines(light_engines),
              light_engines_thrust(light_engines_thrust),
              heavy_engines(heavy_engines),
              heavy_engines_thrust(heavy_engines_thrust),
              longitudinal_top_speed(longitudinal_top_speed),
              angular_top_speed(angular_top_speed),
              angular_acceleration(angular_acceleration),
              lift_force(lift_force),
              vertical_top_speed(vertical_top_speed) {}

 public:
    virtual ~Ship() {}

    inline Weight get_mass() const { return mass; }
    inline int    get_light_engines() const { return light_engines; }
    inline Thrust get_light_engines_thrust() const { return light_engines_thrust; }
    inline int    get_heavy_engines() const { return heavy_engines; }
    inline Thrust get_heavy_engines_thrust() const { return heavy_engines_thrust; }
    inline Speed  get_longitudinal_top_speed() const {
      return longitudinal_top_speed;
    }
    inline Angular_Speed get_angular_top_speed() const { return angular_top_speed; }
    inline Angular_Acceleration get_angular_acceleration() const {
      return angular_acceleration;
    }
    inline Force  get_lift_force() const { return lift_force; }
    inline Speed  get_vertical_top_speed() const { return vertical_top_speed; }

    Thrust        get_thrust() const;
    Acceleration  get_longitudinal_acceleration() const;
    double        get_longitudinal_drag() const;
    Acceleration  get_vertical_acceleration() const;
    double        get_vertical_drag() const;
};

class Pyramidion : public Ship {
 public:
    explicit Pyramidion(const std::string& name) : Ship(
                          name,
                          650_hp,             // armor
                          700_hp,             // hull health
                          300000_kg,          // mass
                          2,                  // number of light engines
                          187002.7_N,         // thrust of light engine
                          1,                  // number of heavy engines
                          299744.4_N,         // thrust of heavy engine
                          30.36_m/1_s,        // top speed
                          11.05_deg/1_s,      // angular top speed
                          6.25_deg_s2,        // angular acceleration
                          825000_N,           // lift force
                          17.03_m/1_s         // vertical top speed
    ) {}
};

class Goldfish : public Ship {
 public:
    explicit Goldfish(const std::string& name) : Ship(
                          name,
                          400_hp,             // armor
                          1100_hp,            // hull health
                          150000_kg,          // mass
                          2,                  // number of light engines
                          183750_N,           // thrust of light engine
                          1,                  // number of heavy engines
                          157500_N,           // thrust of heavy engine
                          40.11_m/1_s,        // top speed
                          13.99_deg/1_s,      // angular top speed
                          12.99_deg_s2,       // angular acceleration
                          487500_N,           // lift force
                          16.99_m/1_s         // vertical top speed
    ) {}
};

class Junker : public Ship {
 public:
    explicit Junker(const std::string& name) : Ship(
                          name,
                          700_hp,             // armor
                          500_hp,             // hull health
                          125000_kg,          // mass
                          2,                  // number of light engines
                          179166.666666_N,    // thrust of light engine
                          1,                  // number of heavy engines
                          179166.666666_N,    // thrust of heavy engine
                          26.01_m/1_s,        // top speed
                          16.18_deg/1_s,      // angular top speed
                          15.24_deg_s2,       // angular acceleration
                          375000_N,           // lift force
                          17.06_m/1_s         // vertical top speed
    ) {}
};

class Galleon : public Ship {
 public:
    explicit Galleon(const std::string& name) : Ship(
                          name,
                          800_hp,             // armor
                          1400_hp,            // hull health
                          320000_kg,          // mass
                          2,                  // number of light engines
                          134400.2_N,         // thrust of light engine
                          1,                  // number of heavy engines
                          403200.6_N,         // thrust of heavy engine
                          30.02_m/1_s,        // top speed
                          5.06_deg/1_s,       // angular top speed
                          8.02_deg_s2,        // angular acceleration
                          720000_N,           // lift force
                          17.01_m/1_s         // vertical top speed
    ) {}
};

class Squid : public Ship {
 public:
    explicit Squid(const std::string& name) : Ship(
                          name,
                          230_hp,             // armor
                          950_hp,             // hull health
                          95000_kg,           // mass
                          4,                  // number of light engines
                          190000_N,           // thrust of light engine
                          0,                  // number of heavy engines
                          0_N,                // thrust of heavy engine
                          47.00_m/1_s,        // top speed
                          19.11_deg/1_s,      // angular top speed
                          20.01_deg_s2,       // angular acceleration
                          380000_N,           // lift force
                          17.00_m/1_s         // vertical top speed
    ) {}
};

class Spire : public Ship {
 public:
    explicit Spire(const std::string& name) : Ship(
                          name,
                          400_hp,             // armor
                          950_hp,             // hull health
                          150000_kg,          // mass
                          2,                  // number of light engines
                          112050_N,         // thrust of light engine
                          1,                  // number of heavy engines
                          449550_N,         // thrust of heavy engine
                          28.01_m/1_s,        // top speed
                          12.08_deg/1_s,      // angular top speed
                          15.06_deg_s2,        // angular acceleration
                          562500_N,           // lift force
                          17.00_m/1_s         // vertical top speed
    ) {}
};

class Mobula : public Ship {
 public:
    explicit Mobula(const std::string& name) : Ship(
                          name,
                          600_hp,             // armor
                          700_hp,             // hull health
                          120000_kg,          // mass
                          2,                  // number of light engines
                          153000_N,           // thrust of light engine
                          1,                  // number of heavy engines
                          204000_N,           // thrust of heavy engine
                          28.00_m/1_s,        // top speed
                          14.02_deg/1_s,      // angular top speed
                          3.5_deg_s2,         // angular acceleration
                          900000_N,           // lift force
                          17.01_m/1_s         // vertical top speed
    ) {}
};

}  // namespace goio

#endif  // SHIPS_H_
