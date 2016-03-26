#include "goioobj.h"
#include "dmg_types.h"

namespace goio {

  GoioObj::~GoioObj() {
    delete hull;
  }

  bool GoioObj::add_health(double health, double cooldown_end) {
    if ((health > 0 && cmp_type == CmpType::HULL) || health == 0)
      return false;
    this->cooldown_end = cooldown_end;
    return set_health_int(this->health + health, this);
  }

  bool GoioObj::add_rebuild(int rebuild_progress) {
    rebuild_state += rebuild_progress;
    if (rebuild_state >= get_rebuild_value()) {
      health = max_health*health_after_rebuild;
      return true;
    }
    return false;
  }

  bool GoioObj::set_health_int(double health, GoioObj* obj) {
    if (obj->health == -1)
      return true;
    if (health > obj->max_health)
      obj->health = obj->max_health;
    else if (health <= 0) {
      obj->health = 0;
      if (obj->cmp_type != CmpType::HULL) {
        obj->rebuild_state = 0;
        obj->cooldown_end = 0;
      }
      return false;
    } else
      obj->health = health;
    return true;
  }

  void GoioObj::set_health(double health) {
    if (health > max_health)
      max_health = health;
    cooldown_end = 0;
    set_health_int(health, this);
  }

  void GoioObj::set_hull_health(double health) {
    if (health == -1) {
      hull->max_health = hull->health = -1;
      return;
    }
    if (health > hull->max_health)
      hull->max_health = health;
    set_health_int(health, hull);
  }

  void GoioObj::reset(bool hull) {
    health = max_health;
    cooldown_end = 0;
    if (hull)
      this->hull->health = this->hull->max_health;
  }

}
