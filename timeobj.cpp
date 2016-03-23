#include "timeobj.h"
#include "goioobj.h"
#include "goioactor.h"

#include <iostream>
#include <iomanip>

namespace goio {

  TimeObj::~TimeObj() {
    for (auto it = registrars.begin(); it != registrars.end(); ++it) {
      delete it->second;
    }
  }

  bool TimeObj::next_event() {
    if (events.empty())
      return false;

    auto it = events.cbegin();
    time = it->first;
    auto funcdata = it->second;

    using namespace std;
    cout << fixed << setprecision(2);
    cout << setw(8) << right << get_time();

    auto ret = funcdata->timedmgfunc(funcdata->obj);

    // update directly associated actors
    auto iterpair = registrars.equal_range(funcdata->obj);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      recalc_next_event(it->second);
    }

    // update to recipient associated actors
    auto iterpair2 = recipients.equal_range(funcdata->obj);
    for (auto it = iterpair2.first; it != iterpair2.second; ++it) {
      // don't update current actor and don't update actions on hold
      if (it->second != funcdata && it->second->time_cur == 0)
        recalc_next_event(it->second);
    }

    cout << setw(13) << right << funcdata->obj->get_name()
         << setw(10) << right << get_cmp_type_string(funcdata->obj->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_health();
    if (funcdata->obj->get_health() == 0)
      cout << setw(3) << right << setprecision(0)
           << funcdata->obj->get_rebuild_state() << setprecision(2) << setw(7);
    else
      cout << setw(10);
    cout << right << get_cmp_type_string(funcdata->obj->get_hull()->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_hull()->get_health();
    if (funcdata->obj->get_hull()->get_health() == 0)
      cout << setw(3) << right << setprecision(0)
           << funcdata->obj->get_hull()->get_rebuild_state();
    cout << endl;

    TimeFunc timefunc = funcdata->timecheckfunc(funcdata->obj, get_time()-funcdata->time_prev);
    if (timefunc != nullptr) {
      funcdata->time_prev = funcdata->time_next;
      funcdata->time_next = time+timefunc();
      funcdata->time_cur = 0;
      register_event(funcdata, funcdata->time_next);
    } else {
      auto iterpair = registrars.equal_range(funcdata->registrar);
      for (auto it = iterpair.first; it != iterpair.second; ++it) {
        if (it->second == funcdata) {
          funcdata->time_cur = get_time();
          break;
        }
      }
      ret = false;
    }
    events.erase(it);
    return ret;
  }

  bool TimeObj::register_event(FuncData* funcdata, double time) {
    events.insert(std::make_pair(time, funcdata));
    return true;
  }

  bool TimeObj::recalc_next_event(FuncData* funcdata) {
    bool res;
    auto old_time_next = funcdata->time_next;

    auto timefunc = funcdata->timecheckfunc(funcdata->obj, get_time()-funcdata->time_prev);
    if (timefunc != nullptr) {
      auto fac = (time-funcdata->time_prev)/(funcdata->time_next-funcdata->time_prev);
      auto comp_time = timefunc();

      funcdata->time_next = time+(1-fac)*comp_time;
      funcdata->time_prev = funcdata->time_next - comp_time;

      register_event(funcdata, funcdata->time_next);
      res = true;
    } else {
      funcdata->time_cur = get_time();
      res = false;
    }

    auto iterpair = events.equal_range(old_time_next);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      if (it->second == funcdata) {
        events.erase(it);
        break;
      }
    }
    return res;
  }

  bool TimeObj::register_event(GoioActor* registrar, TimeDmgFunc timedmgfunc,
                               GoioObj* obj, TimeCheckFunc timecheckfunc,
                               double time, bool rel) {
    double comp_time;
    if (rel)
      comp_time = this->time+time;
    else
      comp_time = time;

    if (comp_time < time)
      return false;

    auto funcdata = new FuncData {registrar, timedmgfunc, obj, get_time(),
                                  comp_time, -1, timecheckfunc};
    events.insert(std::make_pair(comp_time, funcdata));
    registrars.insert(std::make_pair(registrar, funcdata));
    recipients.insert(std::make_pair(obj, funcdata));
    return true;
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

}
