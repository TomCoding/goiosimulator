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

#ifndef HELMTOOLS_H_
#define HELMTOOLS_H_

#include "./goioobj.h"


namespace goio {

class HelmTool : public Object {
 protected:
    const double  thrust;
    const double  angular_drag;
    const double  longitudinal_drag;
    const double  lift_force;
    const double  descent_force;
    const double  vertical_drag;
    const Health_P_Time damage;
    const DmgType dmg_type;
    const CmpType dmg_target;
    const double  target_ign_chance;
    const double  dmg_reduction;
    const DmgType dmg_reduction_type;
    const Time    delay_after;
    const bool    tar_cloud;
    const bool    spot;
    const bool    range;

    HelmTool(double  thrust,
             double  angular_drag,
             double  longitudinal_drag,
             double  lift_force,
             double  descent_force,
             double  vertical_drag,
             Health_P_Time damage,
             DmgType dmg_type,
             CmpType dmg_target,
             double  target_ign_chance,
             double  dmg_reduction,
             DmgType dmg_reduction_type,
             Time    delay_after,
             bool    tar_cloud,
             bool    spot,
             bool    range);

 public:
    virtual ~HelmTool() {}

    inline double  get_thrust() const { return thrust; }
    inline double  get_angular_drag() const { return angular_drag; }
    inline double  get_longitudinal_drag() const { return longitudinal_drag; }
    inline double  get_lift_force() const { return lift_force; }
    inline double  get_descent_force() const { return descent_force; }
    inline double  get_vertical_drag() const { return vertical_drag; }
    inline Health_P_Time get_damage() const { return damage; }
    inline DmgType get_dmg_type() const { return dmg_type; }
    inline CmpType get_dmg_target() const { return dmg_target; }
    inline double  get_target_ign_chance() const{ return target_ign_chance; }
    inline double  get_dmg_reduction() const{ return dmg_reduction; }
    inline DmgType get_dmg_reduction_type() const{ return dmg_reduction_type; }
    inline Time    get_delay_after() const { return delay_after; }
    inline bool    get_tar_cloud() const { return tar_cloud; }
    inline bool    get_spot() const { return spot; }
    inline bool    get_range() const { return range; }
};

//TODO: Use template instead, keep static_assert.
#define NEW_HELMTOOL(TOOL, \
                     thrust, \
                     angular_drag, \
                     longitudinal_drag, \
                     lift_force, \
                     descent_force, \
                     vertical_drag, \
                     damage, \
                     dmg_type, \
                     dmg_target, \
                     target_ign_chance, \
                     dmg_reduction, \
                     dmg_reduction_type, \
                     delay_after, \
                     tar_cloud, \
                     spot, \
                     range) \
  class TOOL : public HelmTool { \
   public: \
      TOOL() : HelmTool(thrust, \
                        angular_drag, \
                        longitudinal_drag, \
                        lift_force, \
                        descent_force, \
                        vertical_drag, \
                        damage, \
                        dmg_type, \
                        dmg_target, \
                        target_ign_chance, \
                        dmg_reduction, \
                        dmg_reduction_type, \
                        delay_after, \
                        tar_cloud, \
                        spot, \
                        range) { \
        static_assert(thrust >= -1, "requirement 'thrust >= 0' not met"); \
        static_assert(angular_drag >= -1, \
                      "requirement 'angular_drag >= -1' not met"); \
        static_assert(longitudinal_drag>= -1, \
                      "requirement 'longitudinal_drag >= -1' not met"); \
        static_assert(vertical_drag>= -1, \
                      "requirement 'vertical_drag >= -1' not met"); \
        static_assert(target_ign_chance>= -1, \
                      "requirement 'target_ign_chance >= -1' not met"); \
        static_assert(dmg_reduction >= -1, \
                      "requirement 'dmg_reduction >= 0' not met"); \
        static_assert(delay_after >= 0_s, \
                      "requirement 'delay_after >= 0' not met"); \
      } \
  }

NEW_HELMTOOL(ChuteVent,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             -20,                 // lift force
             3,                   // descent force
             -0.6,                // vertical drag
             97.5_hp/1_s,         // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(DrogueChute,
             -0.6,                // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             2.5,                 // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             2_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Hydrogen,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             3.5,                 // lift force
             -20,                 // descent force
             -0.6,                // vertical drag
             112.5_hp/1_s,        // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0.5,                 // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(ImpactBumpers,
             -0.6,                // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ARMOR,      // damage target
             0,                   // target ignition chance
             -0.25,               // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             5_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Kerosene,
             1.5,                 // thrust
             3,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             10_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Moonshine,
             2,                   // thrust
             10,                  // angular drag
             -0.5,                // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             40_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(PhoenixClaw,
             0.5,                 // thrust
             -0.65,               // angular drag
             2,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             13_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(TarBarrel,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             20_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             true,                // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(SpyGlass,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             true,                // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(RangeFinder,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             true                 // ability to tell range
);

}  // namespace goio

#endif  // HELMTOOLS_H_
