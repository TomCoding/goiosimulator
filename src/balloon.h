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
    Force cur_lift_force;
    Force cur_descent_force;

    Balloon(const Balloon& obj);
    Balloon& operator=(const Balloon& obj);

    void set_lift_force(Force lift_force);
    void set_descent_force(Force descent_force);

 public:
    Balloon(const std::string& name, Force lift_force,
            Health max_health = 1800_hp) :
              BalloonInfo(lift_force),
              GoioObj(name, CmpType::BALLOON, 0.666666, max_health),
              cur_lift_force(lift_force),
              cur_descent_force(lift_force) {}
    virtual ~Balloon() {}

    inline Force get_lift_force() const { return cur_lift_force; }
    inline Force get_descent_force() const { return cur_descent_force; }

    Force get_lift_force_changed() const;
    Force get_descent_force_changed() const;

    void reset(bool) override;
};

}  // namespace goio


#endif  // BALLOON_H_
