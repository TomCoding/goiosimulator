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
#include <set>

#include "./goioobj.h"
#include "./balloon.h"
#include "./engines.h"
#include "./guns.h"
#include "./utils.h"


namespace goio {

class Shipinfo {
 private:
    const Weight mass;
    const int    light_engines;
    const Thrust light_engine_thrust;
    const int    heavy_engines;
    const Thrust heavy_engine_thrust;
    const double longitudinal_drag;
    const Angular_Speed angular_top_speed;
    const double angular_drag;
    const Force  lift_force;
    const double vertical_drag;

 protected:
    Shipinfo(Weight mass,
             int light_engines,
             Thrust light_engines_thrust,
             int heavy_engines,
             Thrust heavy_engines_thrust,
             double longitudinal_drag,
             Angular_Speed angular_top_speed,
             double angular_drag,
             Force lift_force,
             double vertical_drag);

    Thrust        get_thrust_int(Thrust light_engines_thrust,
                                 Thrust heavy_engines_thrust) const;
    Speed         get_top_speed_int(double drag, Acceleration acceleration) const;
    Acceleration  get_acceleration_int(Force force, Weight mass) const;
    /*
     * For angular speed calculation, more insight on ship's center of mass
     * required.
     */
    Angular_Acceleration get_angular_acceleration_int(double drag,
                                              Angular_Speed top_speed) const;

 public:
    virtual ~Shipinfo() {}

    inline Weight get_orig_mass() const { return mass; }
    inline int    get_light_engines_count() const { return light_engines; }
    inline Thrust get_orig_light_engine_thrust() const {
      return light_engine_thrust;
    }
    inline int    get_heavy_engines_count() const { return heavy_engines; }
    inline Thrust get_orig_heavy_engine_thrust() const {
      return heavy_engine_thrust;
    }
    inline double get_orig_longitudinal_drag() const { return longitudinal_drag; }
    inline Angular_Speed get_orig_angular_top_speed() const {
      return angular_top_speed;
    }
    inline double get_orig_angular_drag() const { return angular_drag; }
    inline Force  get_orig_lift_force() const { return lift_force; }
    inline Force  get_orig_descent_force() const { return lift_force; }
    inline double get_orig_vertical_drag() const { return vertical_drag; }

    Thrust        get_orig_thrust() const;
    Speed         get_orig_longitudinal_top_speed() const;
    Acceleration  get_orig_longitudinal_acceleration() const;
    Angular_Acceleration get_orig_angular_acceleration() const;
    Speed         get_orig_vertical_top_speed() const;
    Acceleration  get_orig_vertical_acceleration() const;
};

class Ship : public Shipinfo, public GoioObj {
 private:
    Weight cur_mass;
    Thrust cur_light_engine_thrust;
    Thrust cur_heavy_engine_thrust;
    double cur_longitudinal_drag;
    Angular_Speed cur_angular_top_speed;
    double cur_angular_drag;
    Force  cur_lift_force;
    Force  cur_descent_force;
    double cur_vertical_drag;

    Balloon* balloon;
    std::set<LightEngine*> engines_l;
    std::set<HeavyEngine*> engines_h;
    std::set<Gun*> guns;

    const HelmTool* cur_tool;

    Ship(const Ship& obj);
    Ship& operator=(const Ship& obj);

    void set_mass(Weight mass);
    void set_light_engine_thrust(Thrust thrust);
    void set_heavy_engine_thrust(Thrust thrust);
    void set_longitudinal_drag(double drag);
    void set_angular_top_speed(Angular_Speed speed);
    void set_angular_drag(double drag);
    void set_lift_force(Force lift_force);
    void set_descent_force(Force descent_force);
    void set_vertical_drag(double drag);

 protected:
    Ship(const std::string& name,
         Health max_health, Health hull_max_health,
         Weight mass,
         int light_engines, Thrust light_engine_thrust,
         int heavy_engines, Thrust heavy_engine_thrust,
         double longitudinal_drag,
         Angular_Speed angular_top_speed,
         double angular_drag,
         Force lift_force,
         double vertical_drag);

 public:
    virtual ~Ship();

    inline Weight get_mass() const { return cur_mass; }
    inline Thrust get_light_engine_thrust() const {
      return cur_light_engine_thrust;
    }
    inline Thrust get_heavy_engine_thrust() const {
      return cur_heavy_engine_thrust;
    }
    inline double get_longitudinal_drag() const { return cur_longitudinal_drag; }
    inline Angular_Speed get_angular_top_speed() const {
      return cur_angular_top_speed;
    }
    inline double get_angular_drag() const { return cur_angular_drag; }
    inline Force  get_lift_force() const { return cur_lift_force; }
    inline Force  get_descent_force() const { return cur_descent_force; }
    inline double get_vertical_drag() const { return cur_vertical_drag; }
    inline const HelmTool* get_tool() const { return cur_tool; }

    bool apply_tool(const HelmTool* tool);

    Thrust        get_thrust() const;
    Speed         get_longitudinal_top_speed() const;
    Acceleration  get_longitudinal_acceleration() const;
    Angular_Acceleration get_angular_acceleration() const;
    Speed         get_lift_top_speed() const;
    Speed         get_descent_top_speed() const;
    Acceleration  get_lift_acceleration() const;
    Acceleration  get_descent_acceleration() const;

    inline Balloon* get_balloon() const { return balloon; }
    inline std::set<LightEngine*> get_light_engines() const { return engines_l; }
    inline std::set<HeavyEngine*> get_heavy_engines() const { return engines_h; }
    inline std::set<Gun*> get_guns() const { return guns; }

    void add_gun(Gun* gun);

    void reset(bool = true) override;
};

#define NEW_SHIP(...) VFUNC(NEW_SHIP, __VA_ARGS__)

//TODO: Use template instead, keep static_assert
// default ship macro
#define NEW_SHIP13(SHIP, \
                 max_health, \
                 hull_max_health, \
                 mass, \
                 light_engines, \
                 light_engine_thrust, \
                 heavy_engines, \
                 heavy_engine_thrust, \
                 longitudinal_drag, \
                 angular_top_speed, \
                 angular_drag, \
                 lift_force, \
                 vertical_drag) \
  NEW_SHIP_BEGIN_BLOCK(SHIP, \
                       max_health, \
                       hull_max_health, \
                       mass, \
                       light_engines, \
                       light_engine_thrust, \
                       heavy_engines, \
                       heavy_engine_thrust, \
                       longitudinal_drag, \
                       angular_top_speed, \
                       angular_drag, \
                       lift_force, \
                       vertical_drag) \
  }

// manual rebuild value ship macro
#define NEW_SHIP14(SHIP, \
                 max_health, \
                 hull_max_health, \
                 mass, \
                 light_engines, \
                 light_engine_thrust, \
                 heavy_engines, \
                 heavy_engine_thrust, \
                 longitudinal_drag, \
                 angular_top_speed, \
                 angular_drag, \
                 lift_force, \
                 vertical_drag, \
                 manual_rebuild) \
  NEW_SHIP_BEGIN_BLOCK(SHIP, \
                       max_health, \
                       hull_max_health, \
                       mass, \
                       light_engines, \
                       light_engine_thrust, \
                       heavy_engines, \
                       heavy_engine_thrust, \
                       longitudinal_drag, \
                       angular_top_speed, \
                       angular_drag, \
                       lift_force, \
                       vertical_drag) \
  NEW_SHIP_MANUAL_REBUILD_BLOCK(manual_rebuild) \
  }

#define NEW_SHIP_BEGIN_BLOCK(SHIP, \
                             max_health, \
                             hull_max_health, \
                             mass, \
                             light_engines, \
                             light_engine_thrust, \
                             heavy_engines, \
                             heavy_engine_thrust, \
                             longitudinal_drag, \
                             angular_top_speed, \
                             angular_drag, \
                             lift_force, \
                             vertical_drag) \
  class SHIP : public Ship { \
   public: \
      explicit SHIP(const std::string& name) : \
                    Ship(name, \
                         max_health, \
                         hull_max_health, \
                         mass, \
                         light_engines, \
                         light_engine_thrust, \
                         heavy_engines, \
                         heavy_engine_thrust, \
                         longitudinal_drag, \
                         angular_top_speed, \
                         angular_drag, \
                         lift_force, \
                         vertical_drag \
                    ) { \
        static_assert(max_health > 0_hp, "requirement 'max_health > 0' not met"); \
        static_assert(hull_max_health > 0_hp, \
                      "requirement 'hull_max_health > 0' not met"); \
        static_assert(light_engines >= 0, \
                      "requirement 'light_engines >= 0' not met"); \
        static_assert(light_engine_thrust >= 0_N, \
                      "requirement 'light_engine_thrust >= 0' not met"); \
        static_assert(heavy_engines >= 0, \
                      "requirement 'heavy_engines >= 0' not met"); \
        static_assert(heavy_engine_thrust >= 0_N, \
                      "requirement 'heavy_engine_thrust >= 0' not met"); \
        static_assert(longitudinal_drag > 0, \
                      "requirement 'longitudinal_drag > 0' not met"); \
        static_assert(angular_top_speed > 0_deg/1_s, \
                      "requirement 'angular_top_speed > 0' not met"); \
        static_assert(angular_drag > 0, \
                      "requirement 'angular_drag > 0' not met"); \
        static_assert(lift_force >= 0_N, \
                      "requirement 'lift_force >= 0' not met"); \
        static_assert(vertical_drag > 0, \
                      "requirement 'vertical_drag > 0' not met"); \
      }

#define NEW_SHIP_MANUAL_REBUILD_BLOCK(manual_rebuild) \
    inline int get_rebuild_value() const override { \
      static_assert(manual_rebuild > 0, \
                    "requirement 'manual_rebuild > 0' not met"); \
      return manual_rebuild; \
    }


NEW_SHIP(Pyramidion,
         650_hp,             // armor
         700_hp,             // hull health
         300000_kg,          // mass
         2,                  // number of light engines
         187002.7_N,         // thrust of light engine
         1,                  // number of heavy engines
         299744.4_N,         // thrust of heavy engine
         0.004882849,        // longitudinal drag
         11.05_deg/1_s,      // angular top speed
         0.102373006,        // angular drag
         825000_N,           // lift force
         0.018964151         // vertical drag
);

NEW_SHIP(Goldfish,
         400_hp,             // armor
         1100_hp,            // hull health
         150000_kg,          // mass
         2,                  // number of light engines
         183750_N,           // thrust of light engine
         1,                  // number of heavy engines
         157500_N,           // thrust of heavy engine
         0.004351036,        // longitudinal drag
         13.99_deg/1_s,      // angular top speed
         0.132740582,        // angular drag
         487500_N,           // lift force
         0.022517833         // vertical drag
);

NEW_SHIP(Junker,
         700_hp,             // armor
         500_hp,             // hull health
         125000_kg,          // mass
         2,                  // number of light engines
         179166.666666_N,    // thrust of light engine
         1,                  // number of heavy engines
         179166.666666_N,    // thrust of heavy engine
         0.012712113,        // longitudinal drag
         16.18_deg/1_s,      // angular top speed
         0.116428132,        // angular drag
         375000_N,           // lift force
         0.020615468         // vertical drag
);

NEW_SHIP(Galleon,
         800_hp,             // armor
         1400_hp,            // hull health
         320000_kg,          // mass
         2,                  // number of light engines
         134400.2_N,         // thrust of light engine
         1,                  // number of heavy engines
         403200.6_N,         // thrust of heavy engine
         0.004660458,        // longitudinal drag
         5.06_deg/1_s,       // angular top speed
         0.626474402,        // angular drag
         720000_N,           // lift force
         0.015552632         // vertical drag
);

NEW_SHIP(Squid,
         230_hp,             // armor
         950_hp,             // hull health
         95000_kg,           // mass
         4,                  // number of light engines
         190000_N,           // thrust of light engine
         0,                  // number of heavy engines
         0_N,                // thrust of heavy engine
         0.007243096,        // longitudinal drag
         19.11_deg/1_s,      // angular top speed
         0.109586160,        // angular drag
         380000_N,           // lift force
         0.027681661,        // vertical drag
         20
);

NEW_SHIP(Spire,
         400_hp,             // armor
         950_hp,             // hull health
         150000_kg,          // mass
         2,                  // number of light engines
         112050_N,           // thrust of light engine
         1,                  // number of heavy engines
         449550_N,           // thrust of heavy engine
         0.011471397,        // longitudinal drag
         12.08_deg/1_s,      // angular top speed
         0.206405421,        // angular drag
         562500_N,           // lift force
         0.025951557         // vertical drag
);

NEW_SHIP(Mobula,
         600_hp,             // armor
         700_hp,             // hull health
         120000_kg,          // mass
         2,                  // number of light engines
         153000_N,           // thrust of light engine
         1,                  // number of heavy engines
         204000_N,           // thrust of heavy engine
         0.010841837,        // longitudinal drag
         14.02_deg/1_s,      // angular top speed
         0.035612463,        // angular drag
         900000_N,           // lift force
         0.051842106         // vertical drag
);

}  // namespace goio

#endif  // SHIPS_H_
