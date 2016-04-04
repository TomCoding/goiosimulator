#ifndef __REPAIRTOOLS_H
#define __REPAIRTOOLS_H

#include "goioobj.h"
#include "goioactor.h"
#include <string>

namespace goio {

  class RepairInfo {
    private:
      const double swing;
      const double heal;
      const int    rebuild_power;
      const int    extinguish;
      const int    fire_immune;
      const double cooldown;

    protected:
      RepairInfo(double swing,
                 double heal,
                 int rebuild_power,
                 int extinguish,
                 int fire_immune,
                 double cooldown) : swing(swing),
                                    heal(heal),
                                    rebuild_power(rebuild_power),
                                    extinguish(extinguish),
                                    fire_immune(fire_immune),
                                    cooldown(cooldown) {}
      virtual ~RepairInfo() {};

    public:
      inline double get_swing() const { return swing; }
      inline double get_heal() const { return heal; }
      inline int    get_rebuild_power() const { return rebuild_power; }
      inline int    get_extinguish() const { return extinguish; }
      inline int    get_fire_immune() const { return fire_immune; }
      inline double get_cooldown() const { return cooldown; }
  };

  class RepairTool : public RepairInfo, public GoioActor {
    private:
      int done; // 0 = normal, 1 = rebuild swings, 2 = done
      double cur_swing;
      double repair_wait;

    protected:
      RepairTool(const std::string& name, double swing, double heal,
                 int rebuild_power, int extinguish, int fire_immune, double cooldown) :
                      RepairInfo(swing, heal, rebuild_power, extinguish, fire_immune, cooldown),
                      GoioActor(name, CmpType::COMPONENTS),
                      done(0), cur_swing(swing), repair_wait(0) {}

    public:
      virtual ~RepairTool() {}

      static constexpr double swing_foreshadowing_delay = 0.1;

      void set_cur_swing(double);
      inline double get_cur_swing() const { return cur_swing; }
      inline double wait_cooldown() const { return repair_wait; }

      bool repair(GoioObj* obj, double time, bool&);

      void reset(bool = true) override;

      TimeFunc get_time_func(const GoioObj*, double, bool&) override;

      inline int get_done() const { return done; }
  };

  class Spanner : public RepairTool {
    public:
      explicit Spanner(const std::string& name) : RepairTool(name, 0.7, 40, 5, 0, 0, 2) {}
  };

  class Mallet : public RepairTool {
    public:
      explicit Mallet(const std::string& name) : RepairTool(name, 0.65, 250, 2, 0, 0, 9) {}
  };

  class PipeWrench : public RepairTool {
    public:
      explicit PipeWrench(const std::string& name) : RepairTool(name, 0.667, 120, 4, 0, 0, 5) {}
  };

  class FireExtinguisher : public RepairTool {
    public:
      explicit FireExtinguisher(const std::string& name) : RepairTool(name, 0.667, 0, 0, 50, 5, 3) {}
  };

  class ChemicalSpray : public RepairTool {
    public:
      explicit ChemicalSpray(const std::string& name) : RepairTool(name, 0.667, 0, 0, 3, 25, 5) {}
  };

  class BuffHammer : public RepairTool {
    public:
      explicit BuffHammer(const std::string& name) : RepairTool(name, 0.667, 0, 0, 0, 0, 0.667) {}
  };

}

#endif // __REPAIRTOOLS_H
