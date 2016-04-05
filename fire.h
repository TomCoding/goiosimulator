#ifndef FIRE_H_
#define FIRE_H_

#include "./goioactor.h"


namespace goio {

class Fire : public GoioActor {
 private:
    static constexpr double firetick = 0.001;

 public:
    Fire() : GoioActor("", CmpType::HULL) {}

    bool burn(GoioObj* obj, double, bool&);
    static inline double get_firetick() { return firetick; }

    TimeFunc get_time_func(const GoioObj* obj, double, bool&) override;
};

}  // namespace goio

#endif  // FIRE_H_
