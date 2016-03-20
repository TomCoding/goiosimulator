#include "values.h"
#include <cmath>

#include <iostream>
#include <iomanip>

namespace goio {

  //Ammunition
  Ammunition::Ammunition(double clipsize, double damage,
              bool proportional_self_damage) {
      this->clipsize = clipsize;
      this->damage = damage;
      this->proportional_self_damage = proportional_self_damage;
    }



  //GunInfo
  GunInfo::GunInfo(int clipsize, double rof, double reload, double direct_dmg,
              DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type) {
      this->clipsize = cur_clipsize = clipsize;
      this->rof = cur_rof = rof;
      this->reload = cur_reload = reload;
      this->direct_dmg = cur_direct_dmg = direct_dmg;
      this->direct_dmg_type = cur_direct_dmg_type = direct_dmg_type;
      this->aoe_dmg = cur_aoe_dmg = aoe_dmg;
      this->aoe_dmg_type = cur_aoe_dmg_type = aoe_dmg_type;

      cur_ammo = nullptr;
  }

  void GunInfo::set_clipsize(double clipsize) {
    if (cur_clipsize < 0)
      this->cur_clipsize = 0;
    else
      this->cur_clipsize = std::round(clipsize);
  }
  void GunInfo::set_rof(double rof) {
    if (cur_rof < 0)
      this->cur_rof = 0;
    else
      this->cur_rof = cur_rof;
  }
  void GunInfo::set_reload(double cur_reload) {
    if (reload < 0)
      this->cur_reload = 0;
    else
      this->cur_reload = cur_reload;
  }
  void GunInfo::set_direct_dmg(double cur_direct_dmg) {
    if (cur_direct_dmg < 0)
      this->cur_direct_dmg = 0;
    else
      this->cur_direct_dmg = cur_direct_dmg;
  }
  void GunInfo::set_direct_dmg_type(DmgType cur_direct_dmg_type) {
    this->cur_direct_dmg_type = cur_direct_dmg_type;
  }
  void GunInfo::set_aoe_dmg(double cur_aoe_dmg) {
    if (cur_aoe_dmg < 0)
      this->cur_aoe_dmg = 0;
    else
      this->cur_aoe_dmg = cur_aoe_dmg;
  }
  void GunInfo::set_aoe_dmg_type(DmgType cur_aoe_dmg_type) {
    this->cur_aoe_dmg_type = cur_aoe_dmg_type;
  }

  void GunInfo::apply_ammunition(Ammunition* ammo) {
    set_clipsize(get_clipsize() * ammo->get_clipsize());
    set_direct_dmg(get_direct_dmg() * ammo->get_damage());
    set_aoe_dmg(get_aoe_dmg() * ammo->get_damage());

    cur_ammo = ammo;
  }

  void GunInfo::reset() {
    set_clipsize(clipsize);
    set_rof(rof);
    set_reload(reload);
    set_direct_dmg(direct_dmg);
    set_direct_dmg_type(direct_dmg_type);
    set_aoe_dmg(aoe_dmg);
    set_aoe_dmg_type(aoe_dmg_type);

    cur_ammo = nullptr;
  }



  //Gun
  Gun::Gun(double max_health,
            int clipsize, double rof, double reload, double direct_dmg,
            DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type) :
              GunInfo(clipsize, rof, reload, direct_dmg,
                    direct_dmg_type, aoe_dmg, aoe_dmg_type) {
    this->max_health = health = max_health;
  }

  void Gun::add_health(double health) {
    this->health += health;
    if (this->health > max_health)
      health = max_health;
    else if (this->health < 0)
      this->health = 0;
  }

  double Gun::shoot(GoioObj* obj, bool aoe, double aoe_range) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "shoot health:" << std::setw(10) << std::right << health;
    if (health == 0)
      return 0;

    dec_clipsize();

    GoioObj* tmpobj;
    if (obj->get_health() > 0)
      tmpobj = obj;
    else
      tmpobj = obj->get_hull();

    tmpobj->add_health(-get_direct_dmg()*dmg_types[tmpobj->get_cmp_type()][get_direct_dmg_type()]);

    if (aoe)
      tmpobj->add_health(-get_aoe_dmg()*dmg_types[tmpobj->get_cmp_type()][get_aoe_dmg_type()]);

    std::cout << std::setw(10) << std::right << tmpobj->get_health() << std::endl;

    if (get_ammo()->get_proportional_self_damage())
      add_health(-get_max_health()/(get_max_clipsize()*get_ammo()->get_clipsize()));

    return 1/get_rof_changed();
  }



  //GoioObj
  GoioObj::GoioObj(CmpType cmp_type, double max_health, double hull_max_health) :
            GoioObj() {
    this->cmp_type = cmp_type;
    this->max_health = health = max_health;

    hull = new GoioObj();
    hull->cmp_type = CmpType::HULL;
    hull->max_health = hull->health = hull_max_health;
    hull->hull = nullptr;
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
}
