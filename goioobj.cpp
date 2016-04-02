#include "goioobj.h"
#include "dmg_types.h"

#include <iostream>

namespace goio {

  GoioObj::~GoioObj() {
    delete hull;
  }

  bool GoioObj::add_health(double health, double cooldown_end) {
    if ((health > 0 && cmp_type == CmpType::HULL) || health == 0)
      return false;
    if (cooldown_end >= 0) {
      this->cooldown_end = cooldown_end;
      // std::cout << "\ncooldownS: " << get_name() << " " << cooldown_end << std::endl
      //           << "                                    ";
    }
    return set_health_int(this->health + health, this);
  }

  bool GoioObj::add_fire(int fire, double immunity_end, double cooldown_end) {
    if (health == 0 || cmp_type == CmpType::HULL)
      return false;
    if (immunity_end >= 0)
      this->immunity_end = immunity_end;
    if (cooldown_end >= 0)
      this->cooldown_end = cooldown_end;

    if (fire_stacks == -1)
      return true;

    auto new_fire = fire_stacks + fire;
    if (new_fire > max_fire_stacks)
      fire_stacks = max_fire_stacks;
    else if (new_fire <= 0)
      fire_stacks = 0;
    else
      fire_stacks = new_fire;
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

  bool GoioObj::set_health_int(double health, GoioObj* obj) {
    if (obj->health == -1)
      return true;
    if (health > obj->max_health)
      obj->health = obj->max_health;
    else if (health <= 0) {
      obj->health = 0;
      if (obj->fire_stacks > 0)
        obj->fire_stacks = 0;
      if (obj->cmp_type != CmpType::HULL) {
        obj->rebuild_state = 0;
        obj->cooldown_end = 0;
        // std::cout << "\ncooldown0: 0" << std::endl
        //           << "        ";
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

  void GoioObj::set_fire(int fire) {
    if (fire > max_fire_stacks)
      fire_stacks = max_fire_stacks;
    else if (fire <= 0)
      fire_stacks = 0;
    else
      fire_stacks = fire;
    immunity_end = 0;
  }

  void GoioObj::reset(bool hull) {
    health = max_health;
    if (fire_stacks > 0)
      fire_stacks = 0;
    cooldown_end = 0;
    immunity_end = 0;
    if (hull)
      this->hull->health = this->hull->max_health;
  }

}
