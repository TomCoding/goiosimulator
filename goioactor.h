#ifndef __GOIOACTOR_H
#define __GOIOACTOR_H

#include "goioobj.h"
#include <string>

namespace goio {

  class GoioActor : public GoioObj {
    public:
      GoioActor(const std::string& name, CmpType cmp_type, double part_type_multiplier = -1,
                double max_health = 0, double hull_max_health = 0) :
                GoioObj(name, cmp_type, part_type_multiplier, max_health,
                        hull_max_health) {}
      virtual ~GoioActor() {}

      virtual TimeFunc get_time_func(const GoioObj*, double, bool&) = 0;
  };

}

#endif // __GOIOACTOR_H
