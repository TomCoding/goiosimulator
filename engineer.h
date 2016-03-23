#ifndef __ENGINEER_H
#define __ENGINEER_H

#include "goioactor.h"
#include "repairtools.h"
#include <string>

namespace goio {

  enum RepairMode {
    CONSTANT_DMG_NO_WAIT,
    FASTEST_HEAL,
    // CONSTANT_DMG_WAIT  // anticipate incoming damage and repair accordingly
    // MIXED  // fastest heal during damage pauses
  };

  class Engineer : public GoioActor {
    private:
      RepairMode mode;

      Mallet* mallet;
      PipeWrench* wrench;
      Spanner* spanner;

      double mallet_treshhold;
      double wrench_treshhold;

      RepairTool* cur_tool;

      Engineer(const Engineer& obj) : GoioActor(obj.get_name(), CmpType::HULL),
                              mode(obj.mode),
                              mallet(new Mallet(obj.mallet->get_name())),
                              wrench(new PipeWrench(obj.wrench->get_name())),
                              spanner(new Spanner(obj.spanner->get_name())),
                              mallet_treshhold(obj.mallet_treshhold),
                              wrench_treshhold(obj.wrench_treshhold),
                              cur_tool(nullptr) {}
      Engineer& operator=(const Engineer&) { return *this; }

      void select_tool(RepairTool* tool);

    public:
      Engineer(const std::string name, RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT);
      ~Engineer();

      inline RepairMode get_mode() const { return mode; }

      inline void set_mode(RepairMode mode) { this->mode = mode; }

      bool repair(GoioObj* obj, bool&);

      TimeFunc get_time_func(const GoioObj*, double, bool&) override;
  };

}

#endif // __ENGINEER_H
