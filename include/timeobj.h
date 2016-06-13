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
    Time time;

    enum class EventType {
      NORMAL,
      FIRE,
      UPDATE,
      END,
      // REPORT
    };

    struct FuncData {
      int id;
      GoioActor* registrar;  // key in registrars
      TimeDmgFunc timedmgfunc;
      DmgState dmg_flags;
      GoioObj* obj;
      Time time_prev;
      Time time_next;    // key in events
      Time time_cur;     // if set, not in events (on hold)
      TimeCheckFunc timecheckfunc;
      EventType type;
    };

    /*
     * Higher priorities have lower values, therefore using comparison works
     * from LOW > HIGH...
     */
    enum Priority {
      VERY_HIGH,  // internal events high priority, don't show
      HIGH,       // internal events normal priority, don't show
      HIGH_SHOW,  // internal events, show before user events
      NORMAL,     // all user events, show
      LOW         // internal events, show after user events
    };

    std::multimap<std::pair<Time, Priority>, FuncData*> events;
    std::multimap<GoioObj*, FuncData*> registrars;
    std::multimap<GoioObj*, FuncData*> recipients;

    void register_event(FuncData* funcdata, Time time, Priority priority);
    void register_burn_event(GoioObj* obj);
    void register_update_event(GoioObj* obj, Time time);
    bool recalc_next_event(FuncData* funcdata, Priority priority);

    static int max_id;

    class EndEvent : public GoioActor {
     private:
        TimeObj* timeobj;

        EndEvent(const EndEvent& obj) = delete;
        EndEvent& operator=(const EndEvent& obj) = delete;

     protected:
        void accept(ToolDispatcher&, const Tool*, bool) override {
          assert(false);
        }

     public:
        explicit EndEvent(TimeObj* timeobj) : GoioActor("", CmpType::HULL),
                                              timeobj(timeobj) {}

        inline DmgState noop(GoioObj*, Time) {
          return DmgState::NONE;
        }

        inline TimeFunc get_time_func(const GoioObj*, Time, bool&) override {
          return nullptr;
        }

        int get_buff_value() const override { return -1; }
        void reset_modifiers() override {}
    };

    class UpdateEvent : public GoioActor {
     private:
        Time update_interval;

     protected:
        void accept(ToolDispatcher&, const Tool*, bool) override {
          assert(false);
        }

     public:
        UpdateEvent(Time update_interval) : GoioActor("", CmpType::HULL),
                                            update_interval(update_interval) {}

        inline Time get_update_interval() const { return update_interval; }

        DmgState update(GoioObj* obj, Time time);

        TimeFunc get_time_func(const GoioObj* obj, Time time, bool& force) override;

        int get_buff_value() const override { return -1; }
        void reset_modifiers() override {}
    };

    void free_funcdatas();

 public:
    TimeObj() : time(0), events(), registrars(), recipients() {}
    ~TimeObj();

    inline Time get_time() const { return time; }
    bool next_event();
    int register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                       DmgState dmg_flags,
                       GoioObj* obj, TimeCheckFunc timecheckfunc,
                       Time time = 0_s, bool rel = true);

    template <typename Actor_t>
    inline int register_repair_event(Actor_t* registrar, GoioObj* obj,
                              Time time = 0_s, bool rel = true) {
      return register_event(registrar,
                            std::bind(&RepairActor::repair, registrar, _1, _2),
                            DmgState::TRANSITIONED |
                                  DmgState::START_TARGET_O |
                                  DmgState::START_FIRE_O,
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }
    template <typename Actor_t>
    inline int register_shoot_event(Actor_t* registrar, GoioObj* obj,
                              Time time = 0_s, bool rel = true) {
      return register_event(registrar,
                            std::bind(static_cast<DmgState (Actor_t::*)
                                                  (GoioObj*, Time)>
                                      (&ShootActor::shoot), registrar, _1, _2),
                            DmgState::TRANSITIONED,
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }
    template <typename Actor_t>
    inline int register_monitor_event(Actor_t* registrar, GoioObj* obj,
                              Time time = 0_s, bool rel = true) {
      return register_event(registrar,
                            std::bind(&Actor_t::monitor, registrar, _1, _2),
                            DmgState::TRANSITIONED,
                            obj,
                            std::bind(&Actor_t::get_time_func, registrar, _1,
                                                                          _2,
                                                                          _3),
                            time,
                            rel);
    }

    bool unregister_event(int id);
    bool unregister_actor(GoioActor* obj);
    void unregister_actor(GoioActor* obj, Time time);

    void reset();
};

}  // namespace goio

#endif  // TIMEOBJ_H_
