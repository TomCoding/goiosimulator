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

#ifndef REPAIRTOOLS_H_
#define REPAIRTOOLS_H_

#include <string>
#include <cassert>

#include "./goioobj.h"
#include "./goioactor.h"
#include "./ammunitions.h"
#include "./helmtools.h"
#include "./engines.h"
#include "./balloon.h"
#include "./guns.h"


namespace goio {

class RepairInfo {
 private:
    const Time   swing;
    const Health heal;
    const int    rebuild_power;
    const int    extinguish;
    const Time   fire_immune;
    const Time   cooldown;
    const int    buff;

 protected:
    RepairInfo(Time swing,
               Health heal,
               int rebuild_power,
               int extinguish,
               Time fire_immune,
               Time cooldown,
               int buff) : swing(swing),
                           heal(heal),
                           rebuild_power(rebuild_power),
                           extinguish(extinguish),
                           fire_immune(fire_immune),
                           cooldown(cooldown),
                           buff(buff) {}
    virtual ~RepairInfo() {}

 public:
    inline Time   get_swing() const { return swing; }
    inline Health get_heal() const { return heal; }
    inline int    get_rebuild_power() const { return rebuild_power; }
    inline int    get_extinguish() const { return extinguish; }
    inline Time   get_fire_immune() const { return fire_immune; }
    inline Time   get_cooldown() const { return cooldown; }
    inline int    get_buff() const { return buff; }

    virtual Time  get_buff_duration(const GoioObj*) const { return 0_s; }
    virtual void  apply_buff(GoioObj*) {}
};

class RepairTool : public RepairInfo, public RepairActor {
 private:
    int done;  // 0 = normal, 1 = rebuild swings, 2 = done
    Time cur_swing;
    Time repair_wait;

    bool action_possible(const GoioObj* const obj) const;

 protected:
    RepairTool(const std::string& name, Time swing, Health heal,
               int rebuild_power, int extinguish, Time fire_immune,
               Time cooldown, int buff) :
                    RepairInfo(swing, heal, rebuild_power, extinguish,
                               fire_immune, cooldown, buff),
                    RepairActor(name, CmpType::COMPONENTS),
                    done(0), cur_swing(swing), repair_wait(0) {}

 public:
    virtual ~RepairTool() {}

    static constexpr double swing_foreshadowing_delay = 0.1;

    void set_cur_swing(Time swing);
    inline Time get_cur_swing() const { return cur_swing; }
    inline Time wait_cooldown() const { return repair_wait; }

    DmgState repair(GoioObj* obj, Time time) override;

    void reset(bool = true) override;

    TimeFunc get_time_func(const GoioObj*, Time, bool&) override;

    inline int get_done() const { return done; }
};

NEW_HELMTOOL(EnginesBuff,
             0.25,                // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             0,                   // lift force
             0,                   // descent force
             0,                   // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_HELMTOOL(BalloonBuff,
             0,                   // thrust
             0,                   // angular drag
             0,                   // longitudinal drag
             1,                   // lift force
             1,                   // descent force
             -0.25,               // vertical drag
             0_hp/1_s,            // damage per second to target
             DmgType::FIRE,       // damage type
             CmpType::BALLOON,    // damage target
             0,                   // target ignition chance
             0,                   // incoming damage reduction
             DmgType::IMPACT,     // reduced damage type
             0_s,                 // deactivation delay
             false,               // produce tar cloud
             false,               // ability to spot
             false                // ability to tell range
);

NEW_AMMUNITION(GunsBuff,
               0,             // magazine
               0.2,           // damage
               0,             // rof
               0,             // aoe radius
               0,             // arming time
               0,             // ign_chance
               0,             // projectile speed
               0,             // shell drop
               0,             // jitter
               0,             // rotation speed
               0,             // fire stacks
               false,         // proportional self damage
               false          // temporary fire immunity
);

class BuffTool : public RepairTool {
 private:
    const Time engines_duration;
    const Time guns_duration;
    const Time armor_duration;
    const Time balloon_duration;

    static const EnginesBuff engines_buff;
    static const GunsBuff guns_buff;
    static const BalloonBuff balloon_buff;

 protected:
    BuffTool(const std::string& name, Time swing, Health heal,
             int rebuild_power, int extinguish, Time fire_immune,
             Time cooldown, int buff,
             Time engines_duration, Time guns_duration,
             Time armor_duration, Time balloon_duration) :
                RepairTool(name, swing, heal, rebuild_power,
                           extinguish, fire_immune, cooldown,
                           buff),
                engines_duration(engines_duration),
                guns_duration(guns_duration),
                armor_duration(armor_duration),
                balloon_duration(balloon_duration) {}

 public:
    virtual ~BuffTool() {}

    Time get_buff_duration(const GoioObj* obj) const override {
      switch (obj->get_cmp_type()) {
        case CmpType::ENGINES:
          return engines_duration;
        case CmpType::GUNS:
          return guns_duration;
        case CmpType::ARMOR:
          return armor_duration;
        case CmpType::BALLOON:
          return balloon_duration;
        case CmpType::HULL:
          return 0_s;
        default:
          assert(false);
      }
    }

    void apply_buff(GoioObj* obj) override {
      switch (obj->get_cmp_type()) {
        case CmpType::ENGINES: {
          Engine* e;
          if ((e = dynamic_cast<Engine*>(obj)) != nullptr) {
            e->add_thrust_mod(engines_buff.get_thrust());
          }
          break;
        }
        case CmpType::GUNS: {
          Gun* g;
          if ((g = dynamic_cast<Gun*>(obj)) != nullptr) {
            g->reload(&guns_buff);
          }
          break;
        }
        case CmpType::ARMOR: {
          break;
        }
        case CmpType::BALLOON: {
          Balloon* b;
          if ((b = dynamic_cast<Balloon*>(obj)) != nullptr) {
            b->add_lift_force_mod(balloon_buff.get_lift_force());
            b->add_descent_force_mod(balloon_buff.get_descent_force());
          }
          break;
        }
        case CmpType::HULL: {
          break;
        }
        default:
          assert(false);
      }
    }
};

#define NEW_REPAIRTOOL(...) VFUNC(NEW_REPAIRTOOL, __VA_ARGS__)

//TODO: Use template instead, keep static_assert
// normal tool macro
#define NEW_REPAIRTOOL7(TOOL, \
                        swing, \
                        heal, \
                        rebuild_power, \
                        extinguish, \
                        fire_immune, \
                        cooldown) \
  class TOOL : public RepairTool { \
   public: \
      explicit TOOL(const std::string& name) : \
                    RepairTool(name, \
                               swing, \
                               heal, \
                               rebuild_power, \
                               extinguish, \
                               fire_immune, \
                               cooldown, \
                               0) { \
  NEW_REPAIRTOOL_STATIC_ASSERTS(swing, \
                                heal, \
                                rebuild_power, \
                                extinguish, \
                                fire_immune, \
                                cooldown) \
    } \
  }

// buff tool macro
#define NEW_REPAIRTOOL12(TOOL, \
                         swing, \
                         heal, \
                         rebuild_power, \
                         extinguish, \
                         fire_immune, \
                         cooldown, \
                         buff, \
                         engines_duration, \
                         guns_duration, \
                         armor_duration, \
                         balloon_duration) \
  class TOOL : public BuffTool { \
   public: \
      explicit TOOL(const std::string& name) : \
                    BuffTool(name, \
                             swing, \
                             heal, \
                             rebuild_power, \
                             extinguish, \
                             fire_immune, \
                             cooldown, \
                             buff, \
                             engines_duration, \
                             guns_duration, \
                             armor_duration, \
                             balloon_duration) { \
  NEW_REPAIRTOOL_STATIC_ASSERTS(swing, \
                                heal, \
                                rebuild_power, \
                                extinguish, \
                                fire_immune, \
                                cooldown) \
  NEW_BUFFTOOL_STATIC_ASSERTS(buff, \
                              engines_duration, \
                              guns_duration, \
                              armor_duration, \
                              balloon_duration) \
    } \
  }

#define NEW_REPAIRTOOL_STATIC_ASSERTS(swing, \
                                      heal, \
                                      rebuild_power, \
                                      extinguish, \
                                      fire_immune, \
                                      cooldown) \
        static_assert(swing >= 0_s, "requirement 'swing >= 0' not met"); \
        static_assert(fire_immune >= 0_s, \
                      "requirement 'fire_immune >= 0' not met"); \
        static_assert(cooldown >= 0_s, "requirement 'cooldown >= 0' not met"); \
        static_assert(cooldown > 0_s || swing > 0_s, \
                      "requirement 'cooldown > 0 || swing > 0' not met");

#define NEW_BUFFTOOL_STATIC_ASSERTS(buff, \
                                    engines_duration, \
                                    guns_duration, \
                                    armor_duration, \
                                    balloon_duration) \
        static_assert(buff > 0, "requirement 'buff > 0' not met"); \
        static_assert(engines_duration > 0_s, \
                      "requirement 'engines_duration > 0' not met"); \
        static_assert(guns_duration > 0_s, \
                      "requirement 'guns_duration > 0' not met"); \
        static_assert(armor_duration > 0_s, \
                      "requirement 'armor_duration > 0' not met"); \
        static_assert(balloon_duration > 0_s, \
                      "requirement 'balloon_duration > 0' not met");

NEW_REPAIRTOOL(Spanner,
               0.7_s,         // swing
               40_hp,         // heal
               5,             // rebuild power
               0,             // fire stacks extinguished
               0_s,           // fire immunity
               2_s            // cooldown
);

NEW_REPAIRTOOL(Mallet,
               0.65_s,        // swing
               250_hp,        // heal
               2,             // rebuild power
               0,             // fire stacks extinguished
               0_s,           // fire immunity
               9_s            // cooldown
);

NEW_REPAIRTOOL(PipeWrench,
               0.667_s,       // swing
               120_hp,        // heal
               4,             // rebuild power
               0,             // fire stacks extinguished
               0_s,           // fire immunity
               5_s            // cooldown
);

NEW_REPAIRTOOL(FireExtinguisher,
               0.667_s,       // swing
               0_hp,          // heal
               0,             // rebuild power
               50,            // fire stacks extinguished
               5_s,           // fire immunity
               3_s            // cooldown
);

NEW_REPAIRTOOL(ChemicalSpray,
               0.667_s,       // swing
               0_hp,          // heal
               0,             // rebuild power
               3,             // fire stacks extinguished
               25_s,          // fire immunity
               5_s            // cooldown
);

NEW_REPAIRTOOL(BuffHammer,
               0.667_s,       // swing
               0_hp,          // heal
               0,             // rebuild power
               0,             // fire stacks extinguished
               0_s,           // fire immunity
               0_s,           // cooldown
               1,             // buff
               120_s,         // buff duration on engines
               20_s,          // buff duration on guns
               120_s,         // buff duration on armor
               90_s           // buff duration on balloon
);

}  // namespace goio

#endif  // REPAIRTOOLS_H_
