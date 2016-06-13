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

#include "./timeobj.h"

#include <iostream>
#include <iomanip>
#include <set>

#include "./goioobj.h"
#include "./goioactor.h"
#include "./fire.h"


namespace goio {

int TimeObj::max_id = 0;

TimeObj::~TimeObj() {
  free_funcdatas();
}

bool TimeObj::next_event() {
  if (events.empty())
    return false;

  auto it = events.cbegin();
  time = it->first.first;
  auto funcdata = it->second;

  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;
  using std::setw;
  using std::right;
  if (it->first.second > Priority::HIGH) {
    cout << fixed << setprecision(2);
    cout << setw(8) << right << get_time();
  }

  auto dmg_state = funcdata->timedmgfunc(funcdata->obj, get_time());

  if (funcdata->type == EventType::END) {
    std::cout << "                            ";
  } else {
    bool fire = funcdata->obj->get_fire_stacks() > 0;
    if (funcdata->type != EventType::UPDATE && dmg_state & DmgState::START_BUFF)
      register_update_event(funcdata->obj, funcdata->obj->get_buff_end());

    // update targeted actor
    {
    auto iterpair = registrars.equal_range(funcdata->obj);
    for (auto it2 = iterpair.first; it2 != iterpair.second; ++it2) {
      if (dmg_state & it2->second->dmg_flags)
        recalc_next_event(it2->second, it->first.second);
    }
    }

    // update to recipient associated actors
    {
    auto iterpair = recipients.equal_range(funcdata->obj);
    for (auto it2 = iterpair.first; it2 != iterpair.second; ++it2) {
      // don't update current actor itself
      if (it2->second != funcdata && dmg_state & it2->second->dmg_flags)
        recalc_next_event(it2->second, it->first.second);
      if (it2->second->type == EventType::FIRE)
        fire = false;
    }
    }
    if (fire)
      register_burn_event(funcdata->obj);

    // update to actor associated actors
    if (dmg_state & DmgState::SELF_ALL) {
      auto iterpair = recipients.equal_range(funcdata->registrar);
      for (auto it2 = iterpair.first; it2 != iterpair.second; ++it2) {
        if (it2->second != funcdata &&
            get_dmg_state_self_reversed(dmg_state) & it2->second->dmg_flags)
          recalc_next_event(it2->second, it->first.second);
      }
    }
  }

  if (it->first.second > Priority::HIGH) {
    cout << setw(13) << right << funcdata->obj->get_name()
         << setw(10) << right
         << get_cmp_type_string(funcdata->obj->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_health()
         << setprecision(0);
    if (funcdata->obj->get_health() == 0_hp) {
      cout << setw(3) << right
           << funcdata->obj->get_rebuild_state() << setw(3);
    } else {
      cout << setw(6);
    }
    cout << right << funcdata->obj->get_fire_stacks();
    cout << setw(4) << right << funcdata->obj->get_buff_state() << setprecision(2);
    if (funcdata->obj->buffed())
      cout << " B" << setw(5);
    else
      cout << setw(7);
    cout << right
         << get_cmp_type_string(funcdata->obj->get_hull()->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_hull()->get_health();
    if (funcdata->obj->get_hull()->get_health() == 0_hp) {
      cout << setw(3) << right << setprecision(0)
           << funcdata->obj->get_hull()->get_rebuild_state();
    }
    cout << endl;
  }

  bool force = true;
  TimeFunc timefunc = funcdata->timecheckfunc(funcdata->obj, 0_s, force);
  if (timefunc != nullptr) {
    funcdata->time_prev = funcdata->time_next;
    funcdata->time_next = get_time()+timefunc();
    funcdata->time_cur = 0_s;
    register_event(funcdata, funcdata->time_next, it->first.second);
  } else {
    switch (funcdata->type) {
      case EventType::FIRE:
      case EventType::UPDATE:
        delete funcdata->registrar;
        unregister_event(funcdata->id);
        return true;
      case EventType::END:
        unregister_actor(static_cast<GoioActor*>(funcdata->obj));
        delete funcdata->registrar;
        unregister_event(funcdata->id);
        return true;
      case EventType::NORMAL: {
        auto iterpair = registrars.equal_range(funcdata->registrar);
        for (auto&& it = iterpair.first; it != iterpair.second; ++it) {
          if (it->second == funcdata) {
            funcdata->time_cur = get_time();
            break;
          }
        }
        break;
      }
      default:
        assert(false);
    }
  }
  events.erase(it);
  return true;
}

bool TimeObj::recalc_next_event(FuncData* funcdata, Priority priority) {
  if (funcdata->time_prev < 0_s)
    return true;

  bool res;
  auto old_time_next = funcdata->time_next;

  bool force = false;
  auto timefunc = funcdata->timecheckfunc(funcdata->obj,
                                          get_time()-funcdata->time_prev,
                                          force);
  if (timefunc != nullptr) {
    auto comp_time = timefunc();
    if (!force) {
      double fac;
      if (funcdata->time_cur > 0_s) {
        fac = (funcdata->time_cur-funcdata->time_prev) /
              (funcdata->time_next-funcdata->time_prev);
        funcdata->time_cur = 0_s;
      } else {
        fac = (get_time()-funcdata->time_prev) /
              (funcdata->time_next-funcdata->time_prev);
      }
      funcdata->time_next = get_time() + comp_time*(1-fac);
      funcdata->time_prev = funcdata->time_next - comp_time;
    } else {
      funcdata->time_next = get_time() + comp_time;
      funcdata->time_prev = get_time();
    }

    if (old_time_next != funcdata->time_next)
      register_event(funcdata, funcdata->time_next, priority);
    res = true;
  } else {
    if (funcdata->time_cur == 0_s)
      funcdata->time_cur = get_time();
    res = false;
  }

  static auto default_pair = std::make_pair(0_s, Priority::NORMAL);
  if (old_time_next != funcdata->time_next || timefunc == nullptr) {
    default_pair.first = old_time_next;
    default_pair.second = priority;
    auto iterpair = events.equal_range(default_pair);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      if (it->second == funcdata) {
        events.erase(it);
        break;
      }
    }
  }
  return res;
}

inline void TimeObj::register_event(FuncData* funcdata, Time time, Priority priority) {
  events.emplace(std::piecewise_construct,
                 std::forward_as_tuple(time, priority),
                 std::forward_as_tuple(funcdata));
}

void TimeObj::register_burn_event(GoioObj* obj) {
  auto comp_time = get_time() + Fire::get_firetick();
  auto fire = new Fire();
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  auto funcdata = new FuncData {++max_id, fire,
                            std::bind(&Fire::burn, fire, _1, _2),
                            DmgState::TRANSITIONED,
                            obj, -1_s, comp_time, -1_s,
                            std::bind(&Fire::get_time_func, fire, _1, _2, _3),
                            EventType::FIRE};
  events.emplace(std::piecewise_construct,
                 std::forward_as_tuple(comp_time, Priority::HIGH),
                 std::forward_as_tuple(funcdata));
  registrars.emplace(fire, funcdata);
  recipients.emplace(obj, funcdata);
}

void TimeObj::register_update_event(GoioObj* obj, Time time) {
  auto update = new UpdateEvent(time - get_time());
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  auto funcdata = new FuncData {++max_id, update,
                            std::bind(&UpdateEvent::update, update, _1, _2),
                            DmgState::TRANSITIONED & DmgState::BUFF,
                            obj, -1_s, time, -1_s,
                            std::bind(&UpdateEvent::get_time_func, update, _1, _2, _3),
                            EventType::UPDATE};
  events.emplace(std::piecewise_construct,
                 std::forward_as_tuple(time, Priority::VERY_HIGH),
                 std::forward_as_tuple(funcdata));
  registrars.emplace(update, funcdata);
  recipients.emplace(obj, funcdata);
}

int TimeObj::register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                            DmgState dmg_flags, GoioObj* obj,
                            TimeCheckFunc timecheckfunc,
                            Time time, bool rel) {
  Time comp_time;
  if (rel)
    comp_time = this->time+time;
  else
    comp_time = time;

  if (comp_time < time)
    return false;

  auto funcdata = new FuncData {++max_id, registrar, timedmgfunc, dmg_flags,
                                obj, -1_s, comp_time, -1_s, timecheckfunc,
                                EventType::NORMAL};
  events.emplace(std::piecewise_construct,
                 std::forward_as_tuple(comp_time, Priority::NORMAL),
                 std::forward_as_tuple(funcdata));
  registrars.emplace(registrar, funcdata);
  recipients.emplace(obj, funcdata);
  return funcdata->id;
}

void TimeObj::free_funcdatas() {
  for (auto it = registrars.begin(); it != registrars.end(); ++it) {
    if (it->second->type != EventType::NORMAL)
      delete it->second->registrar;
    delete it->second;
  }
}

void TimeObj::reset() {
  time = 0_s;
  free_funcdatas();
  events.clear();
  registrars.clear();
  recipients.clear();
}

bool TimeObj::unregister_event(int id) {
  FuncData* funcdata = nullptr;
  for (auto it = events.begin(); it != events.end(); ++it) {
    if (it->second->id == id) {
      events.erase(it);
      break;
    }
  }
  for (auto it = registrars.begin(); it != registrars.end(); ++it) {
    if (it->second->id == id) {
      funcdata = it->second;
      registrars.erase(it);
      break;
    }
  }
  for (auto it = recipients.begin(); it != recipients.end(); ++it) {
    if (it->second->id == id) {
      recipients.erase(it);
      break;
    }
  }
  if (funcdata == nullptr)
    return false;
  delete funcdata;
  return true;
}

bool TimeObj::unregister_actor(GoioActor* actor) {
  bool found = false;
  for (auto it = events.begin(); it != events.end();) {
    if (it->second->registrar == actor)
      it = events.erase(it);
    else
      ++it;
  }
  std::set<FuncData*> funcdatas;
  for (auto it = registrars.begin(); it != registrars.end();) {
    if (it->first == actor) {
      funcdatas.insert(it->second);
      it = registrars.erase(it);
      found = true;
    } else {
      ++it;
    }
  }
  for (auto it = funcdatas.begin(); it != funcdatas.end(); ++it) {
    for (auto it2 = recipients.begin(); it2 != recipients.end(); ++it2) {
      if (it2->second == *it) {
        recipients.erase(it2);
        break;
      }
    }
    delete *it;
  }
  return found;
}

void TimeObj::unregister_actor(GoioActor* actor, Time time) {
  if (time < get_time())
    return;
  auto endevent = new EndEvent(this);
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  auto funcdata = new FuncData {++max_id, endevent,
                                std::bind(&EndEvent::noop, endevent, _1, _2),
                                DmgState::NONE,
                                actor,
                                -1_s,
                                time, -1_s,
                                std::bind(&EndEvent::get_time_func,
                                          endevent,
                                          _1, _2, _3),
                                EventType::END};
  events.emplace(std::piecewise_construct,
                 std::forward_as_tuple(time, Priority::HIGH_SHOW),
                 std::forward_as_tuple(funcdata));
  registrars.emplace(endevent, funcdata);
  recipients.emplace(actor, funcdata);
}

DmgState TimeObj::UpdateEvent::update(GoioObj* obj, Time time) {
  if (obj->get_buff_end() <= time) {
    obj->remove_buff();
    return DmgState::BUFF;
  }
  return DmgState::NONE;
}

TimeFunc TimeObj::UpdateEvent::get_time_func(const GoioObj* obj, Time, bool&) {
  if (obj->buffed())
    return nullptr;
  return std::bind(&UpdateEvent::get_update_interval, this);
}

}  // namespace goio
