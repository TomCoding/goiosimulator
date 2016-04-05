#ifndef MONITOR_H_
#define MONITOR_H_

#include "./goioactor.h"


namespace goio {

class Monitor : public GoioActor {
 private:
    double tick;

 public:
    explicit Monitor(double tick) : GoioActor("", CmpType::HULL), tick(tick) {}

    bool monitor(GoioObj* obj, double, bool&);
    inline double get_tick() const { return tick; }

    TimeFunc get_time_func(const GoioObj* obj, double, bool&) override;
};

}  // namespace goio

#endif  // MONITOR_H_
