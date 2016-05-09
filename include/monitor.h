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

#ifndef MONITOR_H_
#define MONITOR_H_

#include "./goioactor.h"
#include "./exceptions.h"


namespace goio {

class Monitor : public GoioActor {
 private:
    Time tick;

 public:
    explicit Monitor(Time tick);  // throws NonPositiveTime

    DmgState::State monitor(GoioObj*, Time);
    inline Time get_tick() const { return tick; }

    TimeFunc get_time_func(const GoioObj* obj, Time, bool&) override;
};

}  // namespace goio

#endif  // MONITOR_H_
