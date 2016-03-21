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

  int GunInfo::get_max_clipsize() {
    if (get_ammo() != nullptr)
      return clipsize*get_ammo()->get_clipsize();
    return clipsize;
  }

  void GunInfo::apply_ammunition(Ammunition* ammo) {
    set_clipsize(clipsize * ammo->get_clipsize());
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

    GoioObj* tmpobj;
    if (obj->get_health() > 0)
      tmpobj = obj;
    else {
      tmpobj = obj->get_hull();
      if (tmpobj->get_health() == 0) {
        done = true;
        return false;
      }
    }

    dec_clipsize();
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
      add_health(-get_max_health()/get_max_clipsize());

    return true;
  }

  TimeFunc Gun::get_time_func() {
    if (done)
      return nullptr;
    if (get_health() == 0)
      return nullptr;
    if (get_clipsize() > 0)
      return std::bind(&Gun::get_time_per_shot, this);
    reload();
    return std::bind(&Gun::get_reload_changed, this);
  }



  //RepairTool
  bool RepairTool::repair(GoioObj* obj) {
    if (obj->get_health() == obj->get_max_health() ||
        (obj->get_health() == 0 && obj->get_hull()->get_health() == 0)) {
      done = 2;
      std::cout << "                            ";
      return false;
    }

    using namespace std;
    cout << fixed << setprecision(2);
    cout << setw(15) << right << get_name();
    cout << "             ";

    if (obj->get_health() > 0) {
      done = 0;
      obj->add_health(get_heal());
    } else {
      done = 1;
      obj->add_rebuild(get_rebuild_power());
    }

    return true;
  }

  TimeFunc RepairTool::get_time_func() {
    switch (done) {
      case 1:
        return std::bind(&RepairTool::get_swing, this);
      case 2:
        return nullptr;
      default:
        return std::bind(&RepairTool::get_cooldown, this);
    };
  }



  //GoioObj
  GoioObj::GoioObj(std::string name, CmpType cmp_type, int part_type_multiplier,
                   double max_health, double hull_max_health) :
            name(name), cmp_type(cmp_type), max_health(max_health), health(max_health),
            rebuild_state(-1), part_type_multiplier(part_type_multiplier),
            hull(new GoioObj()) {
    hull->max_health = hull->health = hull_max_health;
  }

  GoioObj::~GoioObj() {
    delete hull;
  }

  bool GoioObj::add_health(double health) {
    if (health > 0 && cmp_type == CmpType::HULL)
      return false;

    this->health += health;
    if (this->health > max_health)
      this->health = max_health;
    else if (this->health <= 0) {
      this->health = 0;
      rebuild_state = 0;
      return false;
    }
    return true;
  }

  bool GoioObj::add_rebuild(int rebuild_progress) {
    rebuild_state += rebuild_progress;
    if (rebuild_state >= get_rebuild_value()) {
      health = max_health*health_after_rebuild;
      return true;
    }
    return false;
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

    auto iterpair = registrars.equal_range(funcdata->obj);
    for (auto it = iterpair.first; it != iterpair.second; ++it) {
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
    bool res;
    auto old_time_next = funcdata->time_next;

    auto timefunc = funcdata->timecheckfunc();
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
    for (auto it = registrars.begin(); it != registrars.end(); ++it) {
      delete it->second;
    }
    events.clear();
    registrars.clear();
  }
}
