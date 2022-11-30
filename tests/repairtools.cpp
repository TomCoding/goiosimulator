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

#include "./repairtools.h"

#include <string>

#include "gtest/gtest.h"
#include "./private.h"
#include "./balloon.h"
#include "./tooldispatcher.h"
#include "./helmtools.h"
#include "./ships.h"
#include "./engines.h"
#include "./guns.h"


using namespace goio;

TEST(RepairTools, create) {
  std::string name = "create tools";

  auto spanner = new Spanner(name);
  EXPECT_NE(nullptr, spanner);
  delete spanner;

  auto mallet = new Mallet(name);
  EXPECT_NE(nullptr, mallet);
  delete mallet;

  auto wrench = new PipeWrench(name);
  EXPECT_NE(nullptr, wrench);
  delete wrench;

  auto ext = new FireExtinguisher(name);
  EXPECT_NE(nullptr, ext);
  delete ext;

  auto chem = new ChemicalSpray(name);
  EXPECT_NE(nullptr, chem);
  delete chem;

  auto buff = new BuffHammer(name);
  EXPECT_NE(nullptr, buff);
  delete buff;
}

TEST(RepairTools, createFactory) {
  std::string name = "factory";

  auto repairtool = ObjectFactory::create("RepairTool", name);
  EXPECT_EQ(nullptr, repairtool);
  delete repairtool;

  auto bufftool = ObjectFactory::create("BuffTool", name);
  EXPECT_EQ(nullptr, bufftool);
  delete bufftool;

  auto spanner = ObjectFactory::create("Spanner", name);
  EXPECT_NE(nullptr, spanner);
  delete spanner;

  auto mallet = ObjectFactory::create("Mallet", name);
  EXPECT_NE(nullptr, mallet);
  delete mallet;

  auto wrench = ObjectFactory::create("PipeWrench", name);
  EXPECT_NE(nullptr, wrench);
  delete wrench;

  auto ext = ObjectFactory::create("FireExtinguisher", name);
  EXPECT_NE(nullptr, ext);
  delete ext;

  auto chem = ObjectFactory::create("ChemicalSpray", name);
  EXPECT_NE(nullptr, chem);
  delete chem;

  auto buff = ObjectFactory::create("BuffHammer", name);
  EXPECT_NE(nullptr, buff);
  delete buff;


  auto spanner2 = ObjectFactory::create("Spanner");
  EXPECT_EQ(nullptr, spanner2);
  delete spanner2;
}

class TestTool : public RepairTool {
 public:
    explicit TestTool(const std::string& name) : RepairTool(
              name,
              14.49_s,       // swing duration
              200_hp,        // heal
              0,             // rebuild power
              3,             // extinguished fire stacks
              1_s,           // fire immunity
              3.4_s,         // repair cooldown
              0              // buff progress
    ) {}
};

class TestBuffTool : public BuffTool {
 public:
    explicit TestBuffTool(const std::string& name) : BuffTool(
              name,
              14.49_s,       // swing duration
              200_hp,        // heal
              0,             // rebuild power
              3,             // extinguished fire stacks
              1_s,           // fire immunity
              3.4_s,         // repair cooldown
              1,             // buff progress
              11_s,          // buff duration on engines
              19_s,          // buff duration on guns
              7_s,           // buff duration on armor
              90_s           // buff duration on balloon
    ) {}
};

TEST(RepairTools, values) {
  std::string name = "values";

  TestTool t(name);
  EXPECT_EQ(name, t.get_name());
  EXPECT_EQ(14.49_s, t.get_swing());
  EXPECT_EQ(14.49_s, t.get_cur_swing());
  EXPECT_EQ(200_hp, t.get_heal());
  EXPECT_EQ(0, t.get_rebuild_power());
  EXPECT_EQ(3, t.get_extinguish());
  EXPECT_EQ(1_s, t.get_fire_immune());
  EXPECT_EQ(3.4_s, t.get_cooldown());
  EXPECT_EQ(0, t.get_buff());

  auto obj = new FreeObject("", CmpType::BALLOON);
  EXPECT_EQ(0_s, t.get_buff_duration(obj));

  EXPECT_EQ(0, t.get_done());
  EXPECT_EQ(0_s, t.wait_cooldown());
  EXPECT_EQ(-1, t.get_buff_value());

  t.reset_modifiers();
  EXPECT_EQ(name, t.get_name());
  EXPECT_EQ(14.49_s, t.get_swing());
  EXPECT_EQ(14.49_s, t.get_cur_swing());
  EXPECT_EQ(200_hp, t.get_heal());
  EXPECT_EQ(0, t.get_rebuild_power());
  EXPECT_EQ(3, t.get_extinguish());
  EXPECT_EQ(1_s, t.get_fire_immune());
  EXPECT_EQ(3.4_s, t.get_cooldown());
  EXPECT_EQ(0, t.get_buff());
  EXPECT_EQ(0_s, t.get_buff_duration(obj));

  EXPECT_EQ(0, t.get_done());
  EXPECT_EQ(0_s, t.wait_cooldown());
  EXPECT_EQ(-1, t.get_buff_value());

  TestBuffTool bt(name);
  EXPECT_EQ(name, bt.get_name());
  EXPECT_EQ(14.49_s, bt.get_swing());
  EXPECT_EQ(14.49_s, bt.get_cur_swing());
  EXPECT_EQ(200_hp, bt.get_heal());
  EXPECT_EQ(0, bt.get_rebuild_power());
  EXPECT_EQ(3, bt.get_extinguish());
  EXPECT_EQ(1_s, bt.get_fire_immune());
  EXPECT_EQ(3.4_s, bt.get_cooldown());
  EXPECT_EQ(1, bt.get_buff());
  EXPECT_EQ(11_s, bt.get_engines_duration());
  EXPECT_EQ(19_s, bt.get_guns_duration());
  EXPECT_EQ(7_s, bt.get_armor_duration());
  EXPECT_EQ(90_s, bt.get_balloon_duration());

  auto obj_e = new FreeObject("engine", CmpType::ENGINES);
  EXPECT_EQ(11_s, bt.get_buff_duration(obj_e));
  delete obj_e;

  auto obj_g = new FreeObject("gun", CmpType::GUNS);
  EXPECT_EQ(19_s, bt.get_buff_duration(obj_g));
  delete obj_g;

  auto obj_a = new FreeObject("armor", CmpType::ARMOR);
  EXPECT_EQ(7_s, bt.get_buff_duration(obj_a));
  delete obj_a;

  auto obj_b = new FreeObject("balloon", CmpType::BALLOON);
  EXPECT_EQ(90_s, bt.get_buff_duration(obj_b));
  delete obj_b;

  auto obj_c = new FreeObject("component", CmpType::COMPONENTS);
  EXPECT_EQ(-1_s, bt.get_buff_duration(obj_c));
  delete obj_c;

  auto obj_h = new FreeObject("hull", CmpType::HULL);
  EXPECT_EQ(-1_s, bt.get_buff_duration(obj_h));
  delete obj_h;

  delete obj;
}

TEST(RepairTools, valuesSet) {
  std::string name = "";

  const Time default_swing = 0.7_s;

  Time swing_new = 0.21_s;
  Time swing_new2 = 10_s;

  Spanner sp(name);
  EXPECT_EQ(name, sp.get_name());
  EXPECT_EQ(default_swing, sp.get_swing());
  EXPECT_EQ(default_swing, sp.get_cur_swing());

  sp.set_cur_swing(swing_new);
  EXPECT_EQ(default_swing, sp.get_swing());
  EXPECT_EQ(swing_new, sp.get_cur_swing());

  sp.set_cur_swing(swing_new2);
  EXPECT_EQ(default_swing, sp.get_swing());
  EXPECT_EQ(default_swing, sp.get_cur_swing());

  sp.set_cur_swing(-swing_new);
  EXPECT_EQ(default_swing, sp.get_swing());
  EXPECT_EQ(0_s, sp.get_cur_swing());
}

TEST(RepairTools, reset) {
  Spanner sp("reset");
  Time swing_new = 0.562_s;
  Time default_swing = sp.get_swing();

  sp.set_cur_swing(swing_new);
  EXPECT_EQ(swing_new, sp.get_cur_swing());
  EXPECT_NE(default_swing, sp.get_cur_swing());

  sp.reset(true);
  EXPECT_EQ(default_swing, sp.get_cur_swing());
  EXPECT_EQ(0, sp.get_done());

  auto obj = new FreeObject("", CmpType::ARMOR, -1, 200_hp);
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 10_s));
  EXPECT_EQ(2, sp.get_done());
  sp.reset(true);
  EXPECT_EQ(0, sp.get_done());
  delete obj;
}

TEST(RepairTools, repairDead) {
  Spanner sp("repair");
  sp.set_cur_swing(0.1_s);
  auto obj = new FreeObject("", CmpType::BALLOON, -1, 0_hp, 0_hp);
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 17.5_s));
  EXPECT_EQ(0_hp, obj->get_health());
  EXPECT_EQ(0_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, sp.get_done());
  EXPECT_EQ(sp.get_swing(), sp.get_cur_swing());
  delete obj;
}

TEST(RepairTools, repairNone) {
  Spanner sp("repair2");
  ChemicalSpray chem("repair2");
  auto obj = new FreeObject("", CmpType::BALLOON, -1, 200_hp, 13_hp);

  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 10_s));
  EXPECT_EQ(200_hp, obj->get_health());
  EXPECT_EQ(13_hp, obj->get_hull()->get_health());
  EXPECT_EQ(-1, obj->get_fire_stacks());
  EXPECT_EQ(2, sp.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());
  EXPECT_EQ(sp.get_swing(), sp.get_cur_swing());

  EXPECT_EQ(DmgState::NONE, chem.repair(obj, 0.43_s));
  EXPECT_EQ(200_hp, obj->get_health());
  EXPECT_EQ(13_hp, obj->get_hull()->get_health());
  EXPECT_EQ(-1, obj->get_fire_stacks());
  EXPECT_EQ(2, chem.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());
  EXPECT_EQ(chem.get_swing(), chem.get_cur_swing());


  obj->set_fire(5);
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 0_s));
  EXPECT_EQ(200_hp, obj->get_health());
  EXPECT_EQ(13_hp, obj->get_hull()->get_health());
  EXPECT_EQ(5, obj->get_fire_stacks());
  EXPECT_EQ(2, sp.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());
  EXPECT_EQ(sp.get_swing(), sp.get_cur_swing());

  obj->add_fire(1, -1_s, 5_s);
  EXPECT_EQ(DmgState::NONE, chem.repair(obj, 3.2_s));
  EXPECT_EQ(200_hp, obj->get_health());
  EXPECT_EQ(13_hp, obj->get_hull()->get_health());
  EXPECT_EQ(6, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_DOUBLE_EQ(1.8, chem.wait_cooldown()/1_s);
  EXPECT_EQ(chem.get_swing(), chem.get_cur_swing());


  // sp.get_done() == 2
  obj->add_health(-120_hp);
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 5_s));
  EXPECT_EQ(80_hp, obj->get_health());
  EXPECT_EQ(13_hp, obj->get_hull()->get_health());
  EXPECT_EQ(6, obj->get_fire_stacks());
  EXPECT_EQ(1, sp.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());
  EXPECT_EQ(sp.get_swing(), sp.get_cur_swing());

  delete obj;
}

TEST(RepairTools, repairStackedObj) {
  Spanner sp("repair stacked obj");
  auto obj = new FreeObject("", CmpType::BALLOON, 1, 10_hp);
  auto obj2 = new FreeObject("", CmpType::ARMOR, 1, 21_hp, 33_hp);
  obj->set_hull(obj2);

  // full health
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 0_s));
  EXPECT_EQ(10_hp, obj->get_health());
  EXPECT_EQ(21_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, sp.get_done());
  sp.reset(true);

  // component destroyed
  obj->set_health(0_hp);
  EXPECT_EQ(DmgState::START_REBUILD, sp.repair(obj, 0_s));
  EXPECT_EQ(0_hp, obj->get_health());
  EXPECT_EQ(5, obj->get_rebuild_state());
  EXPECT_EQ(21_hp, obj->get_hull()->get_health());
  EXPECT_EQ(1, sp.get_done());
  sp.reset(true);

  // component + armor destroyed
  obj->reset(true);
  obj->set_health(0_hp);
  obj->set_hull_health(0_hp);
  EXPECT_EQ(DmgState::START_REBUILD, sp.repair(obj, 0_s));
  EXPECT_EQ(0_hp, obj->get_health());
  EXPECT_EQ(5, obj->get_rebuild_state());
  EXPECT_EQ(0_hp, obj->get_hull()->get_health());
  EXPECT_EQ(0, obj->get_hull()->get_rebuild_state());
  EXPECT_EQ(1, sp.get_done());
  sp.reset(true);

  // ship death
  obj->reset(true);
  obj->set_health(0_hp);
  obj->set_hull_health(0_hp);
  obj2->set_hull_health(0_hp);
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 0_s));
  EXPECT_EQ(0_hp, obj->get_health());
  EXPECT_EQ(0_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, sp.get_done());

  delete obj2;
  delete obj;
}

TEST(RepairTools, repair) {
  Spanner sp("repair");
  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  obj->set_health(800_hp);
  obj->set_fire(5);

  // normal repair
  EXPECT_EQ(DmgState::TARGET, sp.repair(obj, 0_s));
  EXPECT_EQ(840_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(5, obj->get_fire_stacks());
  EXPECT_EQ(0, sp.get_done());
  EXPECT_EQ(2_s, sp.wait_cooldown());

  // in cooldown
  EXPECT_EQ(DmgState::NONE, sp.repair(obj, 1_s));
  EXPECT_EQ(840_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(5, obj->get_fire_stacks());
  EXPECT_EQ(0, sp.get_done());
  EXPECT_EQ(1_s, sp.wait_cooldown());

  // repair after cooldown
  EXPECT_EQ(DmgState::TARGET, sp.repair(obj, 2_s));
  EXPECT_EQ(880_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(5, obj->get_fire_stacks());
  EXPECT_EQ(0, sp.get_done());
  EXPECT_EQ(2_s, sp.wait_cooldown());

  delete obj;
}

TEST(RepairTools, extinguish) {
  ChemicalSpray chem("extinguish");
  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  obj->set_health(800_hp);
  obj->set_fire(5);

  // normal extinguish
  EXPECT_EQ(DmgState::START_IMMUNITY | DmgState::FIRE, chem.repair(obj, 0_s));
  EXPECT_EQ(800_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_EQ(5_s, chem.wait_cooldown());

  // in cooldown
  EXPECT_EQ(DmgState::NONE, chem.repair(obj, 1_s));
  EXPECT_EQ(800_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_EQ(4_s, chem.wait_cooldown());

  // extinguish after cooldown
  EXPECT_EQ(DmgState::IMMUNITY | DmgState::FIRE, chem.repair(obj, 5_s));
  EXPECT_EQ(800_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(0, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_EQ(5_s, chem.wait_cooldown());

  // no additional immunity
  obj->add_fire(5, -1_s, 0_s);
  EXPECT_EQ(DmgState::FIRE, chem.repair(obj, 5_s));
  EXPECT_EQ(800_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(2, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_EQ(5_s, chem.wait_cooldown());


  // extinguish right after immunity ends
  EXPECT_EQ(DmgState::START_IMMUNITY | DmgState::FIRE, chem.repair(obj, 30_s));
  EXPECT_EQ(800_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(0, obj->get_fire_stacks());
  EXPECT_EQ(0, chem.get_done());
  EXPECT_EQ(5_s, chem.wait_cooldown());

  delete obj;
}

TEST(RepairTools, buff) {
  BuffHammer buff("buff");
  auto obj = new Balloon("", 100000_N);

  // start buff
  EXPECT_EQ(DmgState::START_PREBUFF, buff.repair(obj, 0_s));
  EXPECT_EQ(1200_hp, obj->get_health());
  EXPECT_EQ(1, obj->get_buff_state());
  EXPECT_EQ(0_s, obj->get_buff_end());
  EXPECT_EQ(0, buff.get_done());
  EXPECT_EQ(0_s, buff.wait_cooldown());

  ASSERT_GT(obj->get_buff_value(), 0);
  for (int i = 2; i < obj->get_buff_value(); ++i) {
    EXPECT_EQ(DmgState::PREBUFF, buff.repair(obj, 0_s));
    EXPECT_EQ(1200_hp, obj->get_health());
    EXPECT_EQ(i, obj->get_buff_state());
    EXPECT_EQ(0_s, obj->get_buff_end());
    EXPECT_EQ(0, buff.get_done());
    EXPECT_EQ(0_s, buff.wait_cooldown());
  }

  EXPECT_EQ(DmgState::START_BUFF, buff.repair(obj, 0_s));
  EXPECT_EQ(1200_hp, obj->get_health());
  EXPECT_EQ(0, obj->get_buff_state());
  EXPECT_EQ(90_s, obj->get_buff_end());
  EXPECT_EQ(0, buff.get_done());
  EXPECT_EQ(0_s, buff.wait_cooldown());

  // start second buff
  EXPECT_EQ(DmgState::START_PREBUFF, buff.repair(obj, 0_s));
  EXPECT_EQ(1200_hp, obj->get_health());
  EXPECT_EQ(1, obj->get_buff_state());
  EXPECT_EQ(90_s, obj->get_buff_end());
  EXPECT_EQ(0, buff.get_done());
  EXPECT_EQ(0_s, buff.wait_cooldown());

  ASSERT_GT(obj->get_buff_value(), 0);
  for (int i = 2; i < obj->get_buff_value(); ++i) {
    EXPECT_EQ(DmgState::PREBUFF, buff.repair(obj, 0_s));
    EXPECT_EQ(1200_hp, obj->get_health());
    EXPECT_EQ(i, obj->get_buff_state());
    EXPECT_EQ(90_s, obj->get_buff_end());
    EXPECT_EQ(0, buff.get_done());
    EXPECT_EQ(0_s, buff.wait_cooldown());
  }

  EXPECT_EQ(DmgState::BUFF, buff.repair(obj, 0_s));
  EXPECT_EQ(1200_hp, obj->get_health());
  EXPECT_EQ(0, obj->get_buff_state());
  EXPECT_EQ(90_s, obj->get_buff_end());
  EXPECT_EQ(0, buff.get_done());
  EXPECT_EQ(0_s, buff.wait_cooldown());

  delete obj;
}

TEST(RepairTools, rebuild) {
  Spanner sp("rebuild");
  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  obj->set_health(0_hp);

  // start rebuild
  EXPECT_EQ(DmgState::START_REBUILD, sp.repair(obj, 0_s));
  EXPECT_EQ(0_hp, obj->get_health());
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(5, obj->get_rebuild_state());
  EXPECT_EQ(1, sp.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());

  for (int i = 10; i < 46; i += 5) {
    EXPECT_EQ(DmgState::REBUILD, sp.repair(obj, 0_s));
    EXPECT_EQ(0_hp, obj->get_health());
    EXPECT_EQ(500_hp, obj->get_hull()->get_health());
    EXPECT_EQ(i, obj->get_rebuild_state());
    EXPECT_EQ(1, sp.get_done());
    EXPECT_EQ(0_s, sp.wait_cooldown());
  }

  EXPECT_EQ(DmgState::TRANSITIONED, sp.repair(obj, 0_s));
  EXPECT_FLOAT_EQ(400, obj->get_health()/1_hp);
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(1, sp.get_done());
  EXPECT_EQ(0_s, sp.wait_cooldown());

  EXPECT_EQ(DmgState::TARGET, sp.repair(obj, 0_s));
  EXPECT_FLOAT_EQ(440, obj->get_health()/1_hp);
  EXPECT_EQ(500_hp, obj->get_hull()->get_health());
  EXPECT_EQ(0, sp.get_done());
  EXPECT_EQ(2_s, sp.wait_cooldown());

  delete obj;
}

class TestToolRepair : public RepairTool {
 public:
    explicit TestToolRepair(const std::string& name) : RepairTool(
              name,
              1_s,           // swing duration
              200_hp,        // heal
              3,             // rebuild power
              0,             // extinguished fire stacks
              0_s,           // fire immunity
              5_s,           // repair cooldown
              0              // buff progress
    ) {}
};

class TestToolBuff : public RepairTool {
 public:
    explicit TestToolBuff(const std::string& name) : RepairTool(
              name,
              0.6_s,         // swing duration
              0_hp,          // heal
              0,             // rebuild power
              0,             // extinguished fire stacks
              0_s,           // fire immunity
              0_s,           // repair cooldown
              1              // buff progress
    ) {}
};

TEST(RepairTools, timefunc_0) {
  TestToolRepair tr("timefunc_0");
  bool force = false;
  TimeFunc timefunc;

  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);

  // object intact
  EXPECT_EQ(0, tr.get_done());
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(5_s, timefunc());
    EXPECT_FALSE(force);
  }

  force = false;
  obj->add_health(-700_hp, 5_s);
  EXPECT_EQ(DmgState::NONE, tr.repair(obj, 3_s));
  EXPECT_EQ(500_hp, obj->get_health());
  EXPECT_EQ(0, tr.get_done());
  EXPECT_EQ(2_s, tr.wait_cooldown());
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(2_s, timefunc());
    EXPECT_FALSE(force);
  }

  TestToolBuff tb("timefunc_0");
  force = false;
  EXPECT_EQ(0, tb.get_done());
  timefunc = tb.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(0.6_s, timefunc());
    EXPECT_FALSE(force);
  }


  // object destroyed
  force = false;
  obj->set_health(0_hp);
  EXPECT_EQ(0, tr.get_done());
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(1_s, timefunc());
    EXPECT_EQ(0_s, tr.wait_cooldown());
    EXPECT_TRUE(force);
  }

  force = false;
  EXPECT_EQ(0, tr.get_done());
  timefunc = tr.get_time_func(obj, 3_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(Time(RepairTool::swing_foreshadowing_delay), timefunc());
    EXPECT_EQ(0_s, tr.wait_cooldown());
    EXPECT_TRUE(force);
  }

  delete obj;
}

TEST(RepairTools, timefunc_1) {
  TestToolRepair tr("timefunc_1");
  bool force = false;
  TimeFunc timefunc;

  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  obj->set_health(0_hp);
  EXPECT_EQ(DmgState::START_REBUILD, tr.repair(obj, 0_s));
  EXPECT_EQ(1, tr.get_done());

  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr)
    EXPECT_EQ(1_s, timefunc());
  EXPECT_FALSE(force);

  tr.set_cur_swing(0.2_s);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr)
    EXPECT_EQ(0.2_s, timefunc());
  EXPECT_FALSE(force);

  delete obj;
}

class TestToolExtinguish : public RepairTool {
 public:
    explicit TestToolExtinguish(const std::string& name) : RepairTool(
              name,
              1_s,           // swing duration
              0_hp,          // heal
              0,             // rebuild power
              5,             // extinguished fire stacks
              1_s,           // fire immunity
              5_s,           // repair cooldown
              0              // buff progress
    ) {}
};

TEST(RepairTools, timefunc_2) {
  TestToolRepair tr("timefunc_2 rep");
  TestToolExtinguish te("timefunc_2 ext");
  bool force;
  TimeFunc timefunc;

  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  EXPECT_EQ(DmgState::NONE, tr.repair(obj, 0_s));
  EXPECT_EQ(2, tr.get_done());
  EXPECT_EQ(DmgState::NONE, te.repair(obj, 0_s));
  EXPECT_EQ(2, te.get_done());

  // nothing to do
  force = false;
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_EQ(nullptr, timefunc);
  EXPECT_FALSE(force);
  EXPECT_EQ(2, tr.get_done());

  force = false;
  obj->set_fire(10);
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_EQ(nullptr, timefunc);
  EXPECT_FALSE(force);
  EXPECT_EQ(2, tr.get_done());

  force = false;
  obj->reset();
  obj->set_health(1000_hp);
  timefunc = te.get_time_func(obj, 0_s, force);
  EXPECT_EQ(nullptr, timefunc);
  EXPECT_FALSE(force);
  EXPECT_EQ(2, te.get_done());

  // normal
  force = false;
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(5_s, timefunc());
    EXPECT_FALSE(force);
  }
  EXPECT_EQ(2, tr.get_done());

  // rebuild
  force = false;
  obj->set_health(0_hp);
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_NE(nullptr, timefunc);
  if (timefunc != nullptr) {
    EXPECT_EQ(Time(RepairTool::swing_foreshadowing_delay), timefunc());
    EXPECT_TRUE(force);
  }
  EXPECT_EQ(2, tr.get_done());

  // dead
  force = false;
  obj->set_hull_health(0_hp);
  timefunc = tr.get_time_func(obj, 0_s, force);
  EXPECT_EQ(nullptr, timefunc);
  EXPECT_FALSE(force);
  EXPECT_EQ(2, tr.get_done());

  delete obj;
}

TEST(RepairTool, acceptTools) {
  TestBuffTool tb("acceptTools");
  ToolDispatcher td;

  auto ship = new Goldfish("");
  tb.accept(td, ship, true);
  delete ship;

  auto engine = new LightEngine("", 100_N);
  tb.accept(td, engine, true);
  delete engine;

  auto balloon = new Balloon("", 2103123_N);
  tb.accept(td, balloon, true);
  delete balloon;

  auto gun = new Banshee("");
  tb.accept(td, gun, true);
  delete gun;
}

TEST(Asserts, RepairTool_repair) {
  TestToolRepair tr("assert");
  bool force = false;
  TimeFunc timefunc;

  auto obj = new FreeObject("", CmpType::BALLOON, 0.666666, 1200_hp, 500_hp);
  tr.*get(RepairTool_f()) = 3;
  EXPECT_EQ(3, tr.get_done());

  // Should just check for ".*Assertion.*" but valgrind removes the output.
  EXPECT_DEATH(tr.get_time_func(obj, 0_s, force)(), ".*Assertion.*|");

  delete obj;
}

TEST(Asserts, RepairTool_buffduration) {
  TestBuffTool br("assert");

  auto obj = new FreeObject("", static_cast<CmpType>(3));
  EXPECT_DEATH(br.get_buff_duration(obj), ".*Assertion.*|");

  delete obj;
}

TEST(Asserts, RepairTool_dispatch) {
  TestToolRepair tr("");
  TestBuffTool tb("");
  const auto tool = new DrogueChute();
  auto ship = new Goldfish("");
  auto engine = new LightEngine("", 100_N);
  auto balloon = new Balloon("", 2103123_N);
  auto gun = new Banshee("");

  ToolDispatcher td;
  EXPECT_DEATH(tr.accept(td, tool, true), ".*Assertion.*|");
  EXPECT_DEATH(tb.accept(td, tool, true), ".*Assertion.*|");
  EXPECT_DEATH(tr.accept(td, ship, true), ".*Assertion.*|");
  EXPECT_DEATH(tr.accept(td, engine, false), ".*Assertion.*|");
  EXPECT_DEATH(tr.accept(td, balloon, true), ".*Assertion.*|");
  EXPECT_DEATH(tr.accept(td, gun, false), ".*Assertion.*|");

  delete ship;
  delete engine;
  delete balloon;
  delete gun;

}
