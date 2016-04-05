#ifndef ENGINEER_H_
#define ENGINEER_H_

#include <string>

#include "./goioactor.h"
#include "./repairtools.h"


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

    RepairTool* tools[3];
    double repair_treshholds[2];

    RepairTool* max_rep_tools[3];

    RepairTool* rebuild_tools[3];
    double rebuild_treshholds[2];

    RepairTool* cur_tool;
    bool delay;

    Engineer(const Engineer& obj) : GoioActor(obj.get_name(), CmpType::HULL),
                            mode(obj.mode),
                            tools(),
                            repair_treshholds(),
                            max_rep_tools(),
                            rebuild_tools(),
                            rebuild_treshholds(),
                            cur_tool(nullptr), delay(false) {}
    Engineer& operator=(const Engineer& obj) {
      if (&obj != this) {
        mode = obj.mode;
        for (int i = 0; i < 3; ++i)
          tools[i] = obj.tools[i];
        for (int i = 0; i < 2; ++i)
          repair_treshholds[i] = obj.repair_treshholds[i];
        for (int i = 0; i < 3; ++i)
          max_rep_tools[i] = obj.max_rep_tools[i];
        for (int i = 0; i < 3; ++i)
          rebuild_tools[i] = obj.rebuild_tools[i];
        for (int i = 0; i < 2; ++i)
          rebuild_treshholds[i] = obj.rebuild_treshholds[i];
        cur_tool = obj.cur_tool;
        delay = obj.delay;
      }
      return *this;
    }

    void select_tool(RepairTool* tool);

 protected:
    Engineer(const std::string& name, RepairMode mode) :
                            GoioActor(name, CmpType::HULL),
                            mode(mode),
                            tools(),
                            repair_treshholds(),
                            max_rep_tools(),
                            rebuild_tools(),
                            rebuild_treshholds(),
                            cur_tool(nullptr), delay(false) {}
    void update_tools(RepairTool* tool1, RepairTool* tool2, RepairTool* tool3);
    void free_tools();

 public:
    Engineer(const std::string& name, RepairTool* tool1, RepairTool* tool2,
             RepairTool* tool3, RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT);
    virtual ~Engineer() {}

    inline RepairMode get_mode() const { return mode; }

    inline void set_mode(RepairMode mode) { this->mode = mode; }

    bool repair(GoioObj* obj, double time, bool&);

    TimeFunc get_time_func(const GoioObj*, double, bool&) override;
};

class MainEngineer : public Engineer {
 public:
    MainEngineer(const std::string& name, bool extinguisher = true,
                 RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT);
    ~MainEngineer();
};

class BuffEngineer : public Engineer {
 public:
    explicit BuffEngineer(const std::string& name, bool extinguisher = true);
    ~BuffEngineer();
};

}  // namespace goio

#endif  // ENGINEER_H_
