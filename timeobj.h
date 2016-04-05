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

#ifndef TIMEOBJ_H_
#define TIMEOBJ_H_

#include <map>

#include "./goioobj.h"
#include "./goioactor.h"


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace goio {

class TimeObj {
 private:
    double time;
    struct FuncData {
      int id;
      GoioObj* registrar;  // key in registrars
      TimeDmgFunc timedmgfunc;
      GoioObj* obj;
      double time_prev;
      double time_next;    // key in events
      double time_cur;     // if set, not in events (on hold)
      TimeCheckFunc timecheckfunc;
      bool fire;
    };

    std::multimap<double, FuncData*> events;
    std::multimap<GoioObj*, FuncData*> registrars;
    std::multimap<GoioObj*, FuncData*> recipients;

    void register_event(FuncData* funcdata, double time);
    void register_burn_event(GoioObj* obj);
    bool recalc_next_event(FuncData* funcdata);

    static int max_id;

 public:
    TimeObj() : time(0), events(), registrars(), recipients() {}
    ~TimeObj();

    inline double get_time() const { return time; }
    bool next_event();
    int register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                        GoioObj* obj, TimeCheckFunc timecheckfunc,
                        double time = 0, bool rel = true);

    template <typename Actor_t>
    inline int register_repair_event(Actor_t* registrar, GoioObj* obj,
                              double time = 0, bool rel = true) {
      return register_event(registrar,
                            std::bind(&Actor_t::repair, registrar, _1, _2, _3),
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }
    template <typename Actor_t>
    inline int register_shoot_event(Actor_t* registrar, GoioObj* obj,
                              double time = 0, bool rel = true) {
      return register_event(registrar,
                            std::bind(static_cast<bool (Actor_t::*)
                                                  (GoioObj*, double, bool&)>
                                      (&Actor_t::shoot), registrar, _1, _2, _3),
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }
    template <typename Actor_t>
    inline int register_monitor_event(Actor_t* registrar, GoioObj* obj,
                              double time = 0, bool rel = true) {
      return register_event(registrar,
                            std::bind(&Actor_t::monitor, registrar, _1, _2, _3),
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }

    bool unregister_event(int id);

    void reset();
};

}  // namespace goio

#endif  // TIMEOBJ_H_
