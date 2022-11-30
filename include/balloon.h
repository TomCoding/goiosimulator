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

#ifndef BALLOON_H_
#define BALLOON_H_

#include <string>

#include "./goioobj.h"
#include "./dmg_types.h"
#include "./exceptions.h"


namespace goio {

class BalloonInfo {
 private:
    const Force lift_force;

 protected:
    explicit BalloonInfo(Force lift_force) : lift_force(lift_force) {}
    virtual ~BalloonInfo() {}

 public:
    inline Force get_orig_lift_force() const { return lift_force; }
    inline Force get_orig_descent_force() const { return lift_force; }
};

class Balloon : public BalloonInfo, public GoioObj {
 private:
    double cur_lift_force_mod;
    double cur_descent_force_mod;

    Balloon(const Balloon& obj) = delete;
    Balloon& operator=(const Balloon& obj) = delete;

 protected:
    void accept(ToolDispatcher& dispatcher, const Tool* tool, bool activate) override {
      tool->accept(dispatcher, this, activate);
    }

 public:
    Balloon(const std::string& name, Force lift_force,
            Health max_health = 1200_hp);  // throws NonPositiveHealth
    virtual ~Balloon() {}

    inline Force get_lift_force() const {
      return get_orig_lift_force() * (1+cur_lift_force_mod);
    }
    inline Force get_descent_force() const {
      return get_orig_descent_force() * (1+cur_descent_force_mod);
    }

    void add_lift_force_mod(double lift_force);
    void add_descent_force_mod(double descent_force);

    Force get_lift_force_changed() const;
    Force get_descent_force_changed() const;

    int get_buff_value() const override { return 10; }
    void reset_modifiers() override;
};

class MineBalloon : public Balloon {
 public:
    explicit MineBalloon(const std::string& name) : Balloon(name, 0_N, 200_hp) {}
};

}  // namespace goio


#endif  // BALLOON_H_
