#ifndef __MONITOR_H
#define __MONITOR_H

#include "goioactor.h"

namespace goio {

  class Monitor : public GoioActor {
    private:
      double tick;

    public:
      Monitor(double tick) : GoioActor("", CmpType::HULL), tick(tick) {}

      bool monitor(GoioObj* obj, double, bool&);
      inline double get_tick() const { return tick; }

      TimeFunc get_time_func(const GoioObj* obj, double, bool&) override;
  };
  
}

#endif // __MONITOR_H
