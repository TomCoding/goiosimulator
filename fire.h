#ifndef __FIRE_H
#define __FIRE_H

#include "goioactor.h"

namespace goio {

  class Fire : public GoioActor {
    public:
      Fire() : GoioActor("", CmpType::HULL) {}

      static constexpr double firetick = 0.001;

      bool burn(GoioObj* obj, double, bool&);
      inline double get_firetick() const { return firetick; }

      TimeFunc get_time_func(const GoioObj* obj, double, bool&) override;
  };
  
}

#endif // __FIRE_H
