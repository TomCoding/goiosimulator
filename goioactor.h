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

#ifndef GOIOACTOR_H_
#define GOIOACTOR_H_

#include <string>

#include "./goioobj.h"

namespace goio {

class GoioActor : public GoioObj {
 protected:
    GoioActor(const std::string& name, CmpType cmp_type,
              double part_type_multiplier = -1,
              double max_health = 0, double hull_max_health = 0) :
              GoioObj(name, cmp_type, part_type_multiplier, max_health,
                      hull_max_health) {}

 public:
    virtual ~GoioActor() {}

    virtual TimeFunc get_time_func(const GoioObj*, double, bool&) = 0;
};

}  // namespace goio

#endif  // GOIOACTOR_H_
