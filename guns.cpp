#include "guns.h"
#include<cmath>

#include <iostream>
#include <iomanip>

namespace goio {

  void Gun::set_clipsize(double clipsize) {
    if (clipsize < 0)
      cur_clipsize = 0;
    else
      cur_clipsize = std::round(clipsize);
  }
  void Gun::set_rof(double rof) {
    if (rof < 0)
      cur_rof = 0;
    else
      cur_rof = rof;
  }
  void Gun::set_reload(double reload) {
    if (reload < 0)
      cur_reload = 0;
    else
      cur_reload = reload;
  }
  void Gun::set_direct_dmg(double direct_dmg) {
    if (direct_dmg < 0)
      cur_direct_dmg = 0;
    else
      cur_direct_dmg = direct_dmg;
  }
  void Gun::set_direct_dmg_type(DmgType direct_dmg_type) {
    cur_direct_dmg_type = direct_dmg_type;
  }
  void Gun::set_aoe_dmg(double aoe_dmg) {
    if (aoe_dmg < 0)
      cur_aoe_dmg = 0;
    else
      cur_aoe_dmg = aoe_dmg;
  }
  void Gun::set_aoe_dmg_type(DmgType aoe_dmg_type) {
    cur_aoe_dmg_type = aoe_dmg_type;
  }
  void Gun::set_aoe_radius(double aoe_radius) {
    if (aoe_radius < 0)
      cur_aoe_radius = 0;
    else
      cur_aoe_radius = aoe_radius;
  }

  int Gun::get_max_clipsize() const {
    if (get_ammo() != nullptr)
      return get_orig_clipsize()*get_ammo()->get_clipsize();
    return get_orig_clipsize();
  }

  void Gun::apply_ammunition(const Ammunition* ammo) {
    set_clipsize(get_orig_clipsize() * ammo->get_clipsize());
    set_direct_dmg(get_orig_direct_dmg() * ammo->get_damage());
    set_aoe_dmg(get_orig_aoe_dmg() * ammo->get_damage());
    set_aoe_radius(get_orig_aoe_radius() * ammo->get_aoe_radius());

    cur_ammo = ammo;
  }

  void Gun::reload(bool with_ammo) {
    set_rof(get_orig_rof());
    set_reload(get_orig_reload());
    set_direct_dmg_type(get_orig_direct_dmg_type());
    set_aoe_dmg_type(get_orig_aoe_dmg_type());

    if (with_ammo)
      apply_ammunition(get_ammo());
    else {
      set_clipsize(get_orig_clipsize());
      set_direct_dmg(get_orig_direct_dmg());
      set_aoe_dmg(get_orig_aoe_dmg());
      set_aoe_radius(get_orig_aoe_radius());
      cur_ammo = nullptr;
    }
  }

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

  TimeFunc Gun::get_time_func(const GoioObj*, double) {
    if (done)
      return nullptr;
    if (get_health() == 0)
      return nullptr;
    if (get_clipsize() > 0)
      return std::bind(&Gun::get_time_per_shot, this);
    reload();
    return std::bind(&Gun::get_reload_changed, this);
  }

}
