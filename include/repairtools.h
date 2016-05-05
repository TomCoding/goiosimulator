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

class Spanner : public RepairTool {
 public:
    explicit Spanner(const std::string& name) :
                        RepairTool(name, 0.7_s, 40_hp, 5, 0, 0_s, 2_s) {}
};

class Mallet : public RepairTool {
 public:
    explicit Mallet(const std::string& name) :
                        RepairTool(name, 0.65_s, 250_hp, 2, 0, 0_s, 9_s) {}
};

class PipeWrench : public RepairTool {
 public:
    explicit PipeWrench(const std::string& name) :
                        RepairTool(name, 0.667_s, 120_hp, 4, 0, 0_s, 5_s) {}
};

class FireExtinguisher : public RepairTool {
 public:
    explicit FireExtinguisher(const std::string& name) :
                        RepairTool(name, 0.667_s, 0_hp, 0, 50, 5_s, 3_s) {}
};

class ChemicalSpray : public RepairTool {
 public:
    explicit ChemicalSpray(const std::string& name) :
                        RepairTool(name, 0.667_s, 0_hp, 0, 3, 25_s, 5_s) {}
};

class BuffHammer : public RepairTool {
 public:
    explicit BuffHammer(const std::string& name) :
                        RepairTool(name, 0.667_s, 0_hp, 0, 0, 0_s, 0.667_s) {}
};

}  // namespace goio

#endif  // REPAIRTOOLS_H_
