/**
 * goiovalues - Guns of Icarus Online damage simulator
 * Copyright (C) 2016  Dominique Lasserre
 *
 * This file is part of goiovalues.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENGINEER_H_
#define ENGINEER_H_

#include <string>

#include "./goioactor.h"
#include "./ai_behavior.h"
#include "./repairtools.h"


namespace goio {

class Engineer : public RepairActor {
 private:
    RepairMode mode;
    ExtinguishMode extmode;

    RepairTool* tools[3];
    Health repair_thresholds[2];

    RepairTool* max_rep_tools[3];

    RepairTool* rebuild_tools[3];
    double rebuild_thresholds[2];

    RepairTool* ext_tools[3];

    RepairTool* cur_tool;
    bool delay;

    Engineer(const Engineer& obj);
    Engineer& operator=(const Engineer& obj);

    void select_tool(RepairTool* tool);
    RepairTool* get_repair_tool(GoioObj* obj);

 protected:
    Engineer(const std::string& name, RepairMode mode, ExtinguishMode extmode) :
                            RepairActor(name, CmpType::HULL),
                            mode(mode),
                            extmode(extmode),
                            tools(),
                            repair_thresholds(),
                            max_rep_tools(),
                            rebuild_tools(),
                            rebuild_thresholds(),
                            ext_tools(),
                            cur_tool(nullptr), delay(false) {}
    void update_tools(RepairTool* tool1, RepairTool* tool2, RepairTool* tool3);
    void free_tools();

    template<typename T>
    void set_tools(T ps1, T ps2, T ps3,
                   RepairTool* tool1,
                   RepairTool* tool2,
                   RepairTool* tool3,
                   RepairTool* *ptr_tools) {
      if (ps1 > ps2) {
        if (ps2 > ps3) {
          ptr_tools[0] = tool1;
          ptr_tools[1] = tool2;
          ptr_tools[2] = tool3;
        } else if (ps1 > ps3) {
          ptr_tools[0] = tool1;
          ptr_tools[1] = tool3;
          ptr_tools[2] = tool2;
        } else {
          ptr_tools[0] = tool3;
          ptr_tools[1] = tool1;
          ptr_tools[2] = tool2;
        }
      } else {
        if (ps1 > ps3) {
          ptr_tools[0] = tool2;
          ptr_tools[1] = tool1;
          ptr_tools[2] = tool3;
        } else if (ps2 > ps3) {
          ptr_tools[0] = tool2;
          ptr_tools[1] = tool3;
          ptr_tools[2] = tool1;
        } else {
          ptr_tools[0] = tool3;
          ptr_tools[1] = tool2;
          ptr_tools[2] = tool1;
        }
      }
    }

    void accept(ToolDispatcher&, const Tool*, bool) override {
      assert(false);
    }

 public:
    Engineer(const std::string& name, RepairTool* tool1, RepairTool* tool2,
             RepairTool* tool3,
             RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT,
             ExtinguishMode extmode = ExtinguishMode::THRESHOLD);
    virtual ~Engineer() {}

    inline RepairMode     get_mode() const { return mode; }
    inline ExtinguishMode get_extmode() const { return extmode; }

    inline void set_mode(RepairMode mode) { this->mode = mode; }
    inline void set_extmode(ExtinguishMode extmode) { this->extmode = extmode; }

    DmgState repair(GoioObj* obj, Time time) override;

    TimeFunc get_time_func(const GoioObj*, Time, bool&) override;

    int get_buff_value() const override { return -1; }
    void reset_modifiers() override {}
};

class MainEngineer : public Engineer {
 public:
    explicit MainEngineer(const std::string& name, bool extinguisher = true,
                          RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT,
                          ExtinguishMode extmode = ExtinguishMode::THRESHOLD);
    ~MainEngineer();
};

class BuffEngineer : public Engineer {
 public:
    explicit BuffEngineer(const std::string& name, bool extinguisher = true,
                          RepairMode mode = RepairMode::CONSTANT_DMG_NO_WAIT,
                          ExtinguishMode extmode = ExtinguishMode::THRESHOLD);
    ~BuffEngineer();
};

}  // namespace goio

#endif  // ENGINEER_H_
