#ifndef __GOIOOBJ_H
#define __GOIOOBJ_H

#include "dmg_types.h"
#include <functional>
#include <cmath>
#include <string>

namespace goio {

  class GoioObj {
    private:
      const std::string name;
      const CmpType cmp_type;
      double max_health;
      double health;
      int rebuild_state;
      const int part_type_multiplier;
      GoioObj* hull;
      double cooldown_end;

      GoioObj(double max_health) : name(""), cmp_type(CmpType::HULL),
                  max_health(max_health), health(max_health), rebuild_state(-1),
                  part_type_multiplier(-1), hull(nullptr), cooldown_end(0) {}
      GoioObj(const GoioObj& obj) : name(obj.name), cmp_type(obj.cmp_type),
                  max_health(obj.max_health), health(obj.max_health),
                  rebuild_state(obj.rebuild_state), part_type_multiplier(obj.part_type_multiplier),
                  hull(obj.hull), cooldown_end(0) {}
      GoioObj& operator=(const GoioObj&) { return *this; };

      bool set_health_int(double health, GoioObj* obj);

    public:
      GoioObj(const std::string name, CmpType cmp_type, int part_type_multiplier = -1,
              double max_health = 0, double hull_max_health = -1) :
              name(name), cmp_type(cmp_type), max_health(max_health), health(max_health),
              rebuild_state(-1), part_type_multiplier(part_type_multiplier),
              hull(new GoioObj(hull_max_health)), cooldown_end(0) {}
      virtual ~GoioObj();

      static const     int    rebuild_base_hits         = 9;
      static constexpr double rebuild_health_multiplier = 0.05;
      static constexpr double health_after_rebuild      = 0.333333333333;

      inline int get_rebuild_value() const {
        return std::round((rebuild_base_hits+max_health*rebuild_health_multiplier)*part_type_multiplier);
      }

      inline const std::string get_name() const { return name; }
      inline CmpType get_cmp_type() const { return cmp_type; }
      inline double  get_max_health() const { return max_health; }
      inline double  get_health() const { return health; }
      inline double  get_rebuild_state() const { return rebuild_state; }
      inline GoioObj* get_hull() const { return hull; }
      inline double  get_cooldown_end() const { return cooldown_end; }

      /*
       * Return `false` if object gets destroyed, otherwise `true`
       */
      bool add_health(double health, double cooldown_end = -1);
      bool add_rebuild(int rebuild_progress);

      void set_health(double health);
      void set_hull_health(double health);

      inline void reset_cooldown() { cooldown_end = 0; }

      virtual void reset(bool hull = true);
  };

  typedef std::function<double ()> TimeFunc;
  typedef std::function<TimeFunc (const GoioObj*, double, bool&)> TimeCheckFunc;

  typedef std::function<bool (GoioObj*, double, bool&)> TimeDmgFunc;

}

#endif // __GOIOOBJ_H
