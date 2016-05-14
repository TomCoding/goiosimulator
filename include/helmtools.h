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
        static_assert(thrust >= 0, "requirement 'thrust >= 0' not met"); \
        static_assert(vertical_drag > 0, \
                      "requirement 'vertical_drag > 0' not met"); \
        static_assert(dmg_reduction >= 0, \
                      "requirement 'dmg_reduction >= 0' not met"); \
        static_assert(delay_after >= 0_s, \
                      "requirement 'delay_after >= 0' not met"); \
      } \
  }

NEW_HELMTOOL(ChuteVent,
             1,                   // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             -19,                 // lift force
             4,                   // descent force
             0.4,                 // vertical drag
             97.5_hp/1_s,         // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(DrogueChute,
             0.4,                 // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             3.5,                 // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             2_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Hydrogen,
             1,                   // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             4.5,                 // lift force
             -19,                 // descent force
             0.4,                 // vertical drag
             112.5_hp/1_s,        // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0.5,                 // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(ImpactBumpers,
             0.4,                 // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ARMOR,      // damage target
             0,                   // target ignition chance
             0.75,                // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             5_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Kerosene,
             2.5,                 // thrust
             4,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             10_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(Moonshine,
             3,                   // thrust
             11,                  // angular drag
             0.5,                 // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             40_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(PhoenixClaw,
             1.5,                 // thrust
             0.35,                // angular drag
             3,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             13_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(TarBarrel,
             1,                   // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             20_hp/1_s,           // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             3_s,                 // deactivation delay
             true,                // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(SpyGlass,
             1,                   // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             true,                // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(RangeFinder,
             1,                   // thrust
             1,                   // angular drag
             1,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             1,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::ENGINES,    // damage target
             0,                   // target ignition chance
             1,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             true                 // ability to tell range
);

}  // namespace goio

#endif  // HELMTOOLS_H_
