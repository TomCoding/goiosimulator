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
  for (auto it = registrars.begin(); it != registrars.end(); ++it) {
    if (it->second->fire)
      delete it->second->registrar;
    delete it->second;
  }
}

bool TimeObj::next_event() {
  if (events.empty())
    return false;

  auto it = events.cbegin();
  time = it->first;
  auto funcdata = it->second;

  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;
  using std::setw;
  using std::right;
  if (!funcdata->fire) {
    cout << fixed << setprecision(2);
    cout << setw(8) << right << get_time();
  }

  auto dmgstate = funcdata->timedmgfunc(funcdata->obj, get_time());

  if (funcdata->end) {
    std::cout << "                            ";
  } else {
    bool fire;
    if (funcdata->obj->get_fire_stacks() > 0)
      fire = true;
    else
      fire = false;

    // update targeted actor
    auto iterpair = registrars.equal_range(funcdata->obj);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      recalc_next_event(it->second);
    }

    // update to recipient associated actors
    auto iterpair2 = recipients.equal_range(funcdata->obj);
    for (auto it = iterpair2.first; it != iterpair2.second; ++it) {
      // don't update current actor itself
      if (it->second != funcdata)
        recalc_next_event(it->second);
      if (it->second->fire)
        fire = false;
    }
    if (fire)
      register_burn_event(funcdata->obj);

    // update to actor associated actors
    if (dmgstate & DmgState::SELF) {
      auto iterpair3 = recipients.equal_range(funcdata->registrar);
      for (auto it = iterpair3.first; it != iterpair3.second; ++it) {
        if (it->second != funcdata)
          recalc_next_event(it->second);
      }
    }
  }

  if (!funcdata->fire) {
    cout << setw(13) << right << funcdata->obj->get_name()
         << setw(10) << right
         << get_cmp_type_string(funcdata->obj->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_health()
         << setprecision(0);
    if (funcdata->obj->get_health() == 0) {
      cout << setw(3) << right
           << funcdata->obj->get_rebuild_state() << setw(3);
    } else {
      cout << setw(6);
    }
    cout << right << funcdata->obj->get_fire_stacks() << setprecision(2);
    cout << setw(7) << right
         << get_cmp_type_string(funcdata->obj->get_hull()->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_hull()->get_health();
    if (funcdata->obj->get_hull()->get_health() == 0) {
      cout << setw(3) << right << setprecision(0)
           << funcdata->obj->get_hull()->get_rebuild_state();
    }
    cout << endl;
  }

  bool force = true;
  TimeFunc timefunc = funcdata->timecheckfunc(funcdata->obj, 0, force);
  if (timefunc != nullptr) {
    funcdata->time_prev = funcdata->time_next;
    funcdata->time_next = time+timefunc();
    funcdata->time_cur = 0;
    register_event(funcdata, funcdata->time_next);
  } else if (funcdata->fire) {
    delete funcdata->registrar;
    unregister_event(funcdata->id);
    return true;
  } else if (funcdata->end) {
    unregister_actor(static_cast<GoioActor*>(funcdata->obj));
    delete funcdata->registrar;
    unregister_event(funcdata->id);
    return true;
  } else {
    auto iterpair = registrars.equal_range(funcdata->registrar);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      if (it->second == funcdata) {
        funcdata->time_cur = get_time();
        break;
      }
    }
    // ret = false;
  }
  events.erase(it);
  // return ret;
  return true;
}

bool TimeObj::recalc_next_event(FuncData* funcdata) {
  if (funcdata->time_prev < 0)
    return true;

  bool res;
  auto old_time_next = funcdata->time_next;
  // auto old_time_prev = funcdata->time_prev;
  // auto old_time_cur = funcdata->time_cur;

  bool force = false;
  auto timefunc = funcdata->timecheckfunc(funcdata->obj,
                                          get_time()-funcdata->time_prev,
                                          force);
  if (timefunc != nullptr) {
    auto comp_time = timefunc();
    if (!force) {
      double fac;
      if (funcdata->time_cur > 0) {
        fac = (funcdata->time_cur-funcdata->time_prev) /
              (funcdata->time_next-funcdata->time_prev);
        funcdata->time_cur = 0;
      } else {
        fac = (get_time()-funcdata->time_prev) /
              (funcdata->time_next-funcdata->time_prev);
      }
      funcdata->time_next = get_time() + (1-fac)*comp_time;
      funcdata->time_prev = funcdata->time_next - comp_time;
    } else {
      funcdata->time_next = get_time() + comp_time;
      funcdata->time_prev = get_time();
    }

    // auto name = funcdata->registrar->get_name();
    // if (14.3 < get_time() && get_time() < 30 &&
    //                       (name != "Gatling1" && name != "Wrench2")) {
    //   std::cout << "\n" << name
    //             << " recalc: " << get_time()
    //             << " old_P:" << old_time_prev
    //             << " old_N:" << old_time_next
    //             << " P:" << funcdata->time_prev
    //             << " N:" << funcdata->time_next
    //             << " old_C:" << old_time_cur
    //             << " C:" << funcdata->time_cur
    //             << " comp_time:" << comp_time << std::endl;
    //   std::cout << "                            ";
    // }

    if (old_time_next != funcdata->time_next)
      register_event(funcdata, funcdata->time_next);
    res = true;
  } else {
    if (funcdata->time_cur == 0) {
      funcdata->time_cur = get_time();
      // if (get_time() > 81) {
      //   std::cout << std::endl << funcdata->registrar->get_name()
      //             << " cur_time:" << funcdata->time_cur << std::endl;
      //   std::cout << "                            ";
      // }
    }
    // else if (/*100 > get_time() &&*/ get_time() > 81) {
    // std::cout << std::endl << funcdata->registrar->get_name()
    //           << " cur_time still:" << funcdata->time_cur << std::endl;
    // std::cout << "                            ";
    // }
    res = false;
  }

  if (old_time_next != funcdata->time_next || timefunc == nullptr) {
    auto iterpair = events.equal_range(old_time_next);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      if (it->second == funcdata) {
        events.erase(it);
        break;
      }
    }
  }
  return res;
}

void TimeObj::register_event(FuncData* funcdata, double time) {
  events.insert(std::make_pair(time, funcdata));
}

void TimeObj::register_burn_event(GoioObj* obj) {
  auto comp_time = get_time() + Fire::get_firetick();
  auto fire = new Fire();
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  auto funcdata = new FuncData {++max_id, fire,
                            std::bind(&Fire::burn, fire, _1, _2),
                            obj, -1, comp_time, -1,
                            std::bind(&Fire::get_time_func, fire, _1, _2, _3),
                            true, false};
  events.insert(std::make_pair(comp_time, funcdata));
  registrars.insert(std::make_pair(fire, funcdata));
  recipients.insert(std::make_pair(obj, funcdata));
}

int TimeObj::register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                            GoioObj* obj, TimeCheckFunc timecheckfunc,
                            double time, bool rel) {
  double comp_time;
  if (rel)
    comp_time = this->time+time;
  else
    comp_time = time;

  if (comp_time < time)
    return false;

  auto funcdata = new FuncData {++max_id, registrar, timedmgfunc, obj, -1,
                                comp_time, -1, timecheckfunc, false, false};
  events.insert(std::make_pair(comp_time, funcdata));
  registrars.insert(std::make_pair(registrar, funcdata));
  recipients.insert(std::make_pair(obj, funcdata));
  return funcdata->id;
}

void TimeObj::reset() {
  time = 0;
  for (auto it = registrars.begin(); it != registrars.end(); ++it) {
    delete it->second;
  }
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

void TimeObj::unregister_actor(GoioActor* actor, double time) {
  if (time < get_time())
    return;
  auto endevent = new EndEvent(this);
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  auto funcdata = new FuncData {++max_id, endevent,
                                std::bind(&EndEvent::noop, endevent, _1, _2),
                                actor,
                                -1,
                                time, -1,
                                std::bind(&EndEvent::get_time_func,
                                          endevent,
                                          _1, _2, _3),
                                false,
                                true};
  events.insert(std::make_pair(time, funcdata));
  registrars.insert(std::make_pair(endevent, funcdata));
  recipients.insert(std::make_pair(actor, funcdata));
}

}  // namespace goio
