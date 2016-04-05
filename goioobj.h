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

#ifndef GOIOOBJ_H_
#define GOIOOBJ_H_

#include <functional>
#include <cmath>
#include <string>

#include "./dmg_types.h"


namespace goio {

class GoioObj {
 private:
    const std::string name;
    const CmpType cmp_type;
    double max_health;
    double health;
    int fire_stacks;
    int rebuild_state;
    const double part_type_multiplier;
    GoioObj* hull;
    double cooldown_end;
    double immunity_end;

    static const int max_fire_stacks = 20;
    static const int fire_stacks_unusable = 8;

    explicit GoioObj(double max_health) : name(""), cmp_type(CmpType::HULL),
                max_health(max_health), health(max_health),
                fire_stacks(-1), rebuild_state(-1),
                part_type_multiplier(-1),
                hull(nullptr), cooldown_end(0), immunity_end(0) {}
    GoioObj(const GoioObj& obj) : name(obj.name), cmp_type(obj.cmp_type),
                max_health(obj.max_health), health(obj.max_health),
                fire_stacks(obj.fire_stacks), rebuild_state(obj.rebuild_state),
                part_type_multiplier(obj.part_type_multiplier),
                hull(nullptr), cooldown_end(obj.cooldown_end),
                immunity_end(obj.immunity_end) {
      hull = new GoioObj(obj.hull->max_health);
      hull->health = obj.hull->health;
      hull->rebuild_state = obj.hull->rebuild_state;
    }
    GoioObj& operator=(const GoioObj& obj) {
      if (&obj != this) {
        max_health = obj.max_health;
        health = obj.health;
        fire_stacks = obj.fire_stacks;
        rebuild_state = obj.rebuild_state;
        hull = obj.hull;
        cooldown_end = obj.cooldown_end;
        immunity_end = obj.immunity_end;
      }
      return *this;
    }

    bool set_health_int(double health, GoioObj* obj);

 public:
    GoioObj(const std::string& name, CmpType cmp_type,
            double part_type_multiplier = -1,
            double max_health = 0, double hull_max_health = -1) :
            name(name), cmp_type(cmp_type), max_health(max_health),
            health(max_health), fire_stacks(-1), rebuild_state(-1),
            part_type_multiplier(part_type_multiplier),
            hull(new GoioObj(hull_max_health)),
            cooldown_end(0), immunity_end(0) {}
    virtual ~GoioObj();

    static const     int    rebuild_base_hits         = 9;
    static constexpr double rebuild_health_multiplier = 0.05;
    static constexpr double health_after_rebuild      = 0.333333333333;

    inline int get_rebuild_value() const {
      return std::round((rebuild_base_hits+max_health *
                         rebuild_health_multiplier) *
                        part_type_multiplier);
    }

    inline const std::string get_name() const { return name; }
    inline CmpType get_cmp_type() const { return cmp_type; }
    inline double  get_max_health() const { return max_health; }
    inline double  get_health() const { return health; }
    inline int     get_fire_stacks() const { return fire_stacks; }
    inline double  get_rebuild_state() const { return rebuild_state; }
    inline GoioObj* get_hull() const { return hull; }
    inline double  get_cooldown_end() const { return cooldown_end; }
    inline double  get_immunity_end() const { return immunity_end; }

    static inline int get_max_fire_stacks() { return max_fire_stacks; }
    static inline int get_fire_stacks_unusable() { return fire_stacks_unusable; }

    /*
     * Return `false` if object gets destroyed, otherwise `true`
     */
    bool add_health(double health, double cooldown_end = -1);
    bool add_fire(int fire, double immunity_end = -1, double cooldown_end = -1);
    bool add_rebuild(int rebuild_progress);

    void set_health(double health);
    void set_hull_health(double health);
    void set_fire(int fire);

    inline void reset_cooldown() { cooldown_end = 0; }

    virtual void reset(bool hull = true);
};

typedef std::function<double ()> TimeFunc;
typedef std::function<TimeFunc (const GoioObj*, double, bool&)> TimeCheckFunc;

typedef std::function<bool (GoioObj*, double, bool&)> TimeDmgFunc;

}  // namespace goio

#endif  // GOIOOBJ_H_
