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

#include "./goioobj.h"
#include "./goioactor.h"


namespace goio {

class RepairInfo {
 private:
    const Time   swing;
    const Health heal;
    const int    rebuild_power;
    const int    extinguish;
    const Time   fire_immune;
    const Time   cooldown;

 protected:
    RepairInfo(Time swing,
               Health heal,
               int rebuild_power,
               int extinguish,
               Time fire_immune,
               Time cooldown) : swing(swing),
                                heal(heal),
                                rebuild_power(rebuild_power),
                                extinguish(extinguish),
                                fire_immune(fire_immune),
                                cooldown(cooldown) {}
    virtual ~RepairInfo() {}

 public:
    inline Time   get_swing() const { return swing; }
    inline Health get_heal() const { return heal; }
    inline int    get_rebuild_power() const { return rebuild_power; }
    inline int    get_extinguish() const { return extinguish; }
    inline Time   get_fire_immune() const { return fire_immune; }
    inline Time   get_cooldown() const { return cooldown; }
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
               Time cooldown) :
                    RepairInfo(swing, heal, rebuild_power, extinguish,
                               fire_immune, cooldown),
                    RepairActor(name, CmpType::COMPONENTS),
                    done(0), cur_swing(swing), repair_wait(0) {}

 public:
    virtual ~RepairTool() {}

    static constexpr double swing_foreshadowing_delay = 0.1;

    void set_cur_swing(Time swing);
    inline Time get_cur_swing() const { return cur_swing; }
    inline Time wait_cooldown() const { return repair_wait; }

    DmgState::State repair(GoioObj* obj, Time time) override;

    void reset(bool = true) override;

    TimeFunc get_time_func(const GoioObj*, Time, bool&) override;

    inline int get_done() const { return done; }
};

//TODO: Use template instead, keep static_assert
#define NEW_REPAIRTOOL(TOOL, \
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
                               cooldown \
                    ) { \
        static_assert(swing >= 0_s, "requirement 'swing >= 0' not met"); \
        static_assert(fire_immune >= 0_s, \
                      "requirement 'fire_immune >= 0' not met"); \
        static_assert(cooldown >= 0_s, "requirement 'cooldown >= 0' not met"); \
      } \
  }

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
               0_s            // cooldown
);

}  // namespace goio

#endif  // REPAIRTOOLS_H_
