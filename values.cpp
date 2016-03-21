#include "values.h"
#include <cmath>

#include <iostream>
#include <iomanip>

namespace goio {

  //GunInfo
  void GunInfo::set_clipsize(double clipsize) {
    if (clipsize < 0)
      this->cur_clipsize = 0;
    else
      this->cur_clipsize = std::round(clipsize);
  }
  void GunInfo::set_rof(double rof) {
    if (rof < 0)
      this->cur_rof = 0;
    else
      this->cur_rof = rof;
  }
  void GunInfo::set_reload(double reload) {
    if (reload < 0)
      this->cur_reload = 0;
    else
      this->cur_reload = reload;
  }
  void GunInfo::set_direct_dmg(double direct_dmg) {
    if (direct_dmg < 0)
      this->cur_direct_dmg = 0;
    else
      this->cur_direct_dmg = direct_dmg;
  }
  void GunInfo::set_direct_dmg_type(DmgType direct_dmg_type) {
    this->cur_direct_dmg_type = direct_dmg_type;
  }
  void GunInfo::set_aoe_dmg(double aoe_dmg) {
    if (aoe_dmg < 0)
      this->cur_aoe_dmg = 0;
    else
      this->cur_aoe_dmg = aoe_dmg;
  }
  void GunInfo::set_aoe_dmg_type(DmgType aoe_dmg_type) {
    this->cur_aoe_dmg_type = aoe_dmg_type;
  }
  void GunInfo::set_aoe_radius(double aoe_radius) {
    if (aoe_radius < 0)
      this->cur_aoe_radius = 0;
    else
      this->cur_aoe_radius = aoe_radius;
  }

  void GunInfo::apply_ammunition(Ammunition* ammo) {
    set_clipsize(get_max_clipsize() * ammo->get_clipsize());
    set_direct_dmg(get_direct_dmg() * ammo->get_damage());
    set_aoe_dmg(get_aoe_dmg() * ammo->get_damage());

    cur_ammo = ammo;
  }

  void GunInfo::reload(bool with_ammo) {
    set_clipsize(clipsize);
    set_rof(rof);
    set_reload(reload_);
    set_direct_dmg(direct_dmg);
    set_direct_dmg_type(direct_dmg_type);
    set_aoe_dmg(aoe_dmg);
    set_aoe_dmg_type(aoe_dmg_type);

    if (with_ammo)
      apply_ammunition(get_ammo());
    else
      cur_ammo = nullptr;
  }



  //Gun
  bool Gun::shoot(GoioObj* obj, bool aoe, double aoe_range) {
    if (get_health() == 0 || get_clipsize() == 0)
      return false;

    using namespace std;
    cout << fixed << setprecision(2);
    cout << setw(15) << right << get_name();
    cout << setw(5) << get_clipsize();
    cout << setw(8) << get_health();

    dec_clipsize();

    GoioObj* tmpobj;
    if (obj->get_health() > 0)
      tmpobj = obj;
    else
      tmpobj = obj->get_hull();

    tmpobj->add_health(-get_direct_dmg()*dmg_types[tmpobj->get_cmp_type()][get_direct_dmg_type()]);

    if (aoe && aoe_range <= get_aoe_radius()) {
      double falloff;
      if (aoe_range/2 <= get_aoe_radius())
        falloff = 1;
      else
        falloff = 1.8-1.6*(aoe_range/get_aoe_radius());
      tmpobj->add_health(-falloff*get_aoe_dmg()*dmg_types[tmpobj->get_cmp_type()][get_aoe_dmg_type()]);
    }

    if (get_ammo()->get_proportional_self_damage())
      add_health(-get_max_health()/(get_max_clipsize()*get_ammo()->get_clipsize()));

    return true;
  }

  TimeFunc Gun::get_time_func() {
    if (get_health() == 0)
      return nullptr;
    if (get_clipsize() > 0)
      return std::bind(&Gun::get_time_per_shot, this);
    reload();
    return std::bind(&Gun::get_reload_changed, this);
  }



  //GoioObj
  GoioObj::GoioObj(std::string name, CmpType cmp_type, double max_health, double hull_max_health) :
            name(name), cmp_type(cmp_type), max_health(max_health), health(max_health),
            hull(new GoioObj()) {
    hull->max_health = hull->health = hull_max_health;
  }

  GoioObj::~GoioObj() {
    delete hull;
  }

  bool GoioObj::add_health(double health) {
    this->health += health;
    if (this->health > max_health)
      this->health = max_health;
    else if (this->health < 0) {
      this->health = 0;
      return false;
    }
    return true;
  }

  void GoioObj::reset() {
    health = max_health;
  }

  void GoioObj::reset_all() {
    reset();
    hull->health = hull->max_health;
  }



  //TimeObj
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

    cout << setw(13) << right << funcdata->obj->get_name()
         << setw(8) << right << get_cmp_type_string(funcdata->obj->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_health()
         << setw(10) << right << get_cmp_type_string(funcdata->obj->get_hull()->get_cmp_type())
         << setw(8) << right << funcdata->obj->get_hull()->get_health() << endl;

    TimeFunc timefunc = funcdata->timecheckfunc();
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
    auto fac = (time-funcdata->time_prev)/(funcdata->time_next-funcdata->time_prev);
    auto comp_time = funcdata->timecheckfunc()();

    auto iterpair = events.equal_range(funcdata->time_next);

    funcdata->time_next = time+(1-fac)*comp_time;
    funcdata->time_prev = funcdata->time_next - comp_time;

    for (auto it = iterpair.first; it != iterpair.second; ++it) {
      if (it->second == funcdata) {
        events.erase(it);
        break;
      }
    }
    register_event(funcdata, funcdata->time_next);
    return true;
  }

  bool TimeObj::register_event(GoioObj* registrar, TimeDmgFunc timedmgfunc,
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
    return true;
  }

  void TimeObj::reset() {
    time = 0;
    for (auto it = events.begin(); it != events.end(); ++it) {
      delete it->second;
    }
    events.clear();
  }
}
