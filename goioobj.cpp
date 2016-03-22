#include "goioobj.h"
#include "dmg_types.h"

namespace goio {

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

  void GoioObj::reset(bool hull) {
    health = max_health;
    if (hull)
      this->hull->health = this->hull->max_health;
  }

}
