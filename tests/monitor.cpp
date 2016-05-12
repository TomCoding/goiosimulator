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

#include "./monitor.h"

#include "gtest/gtest.h"


using namespace goio;

void create_monitor(Time time);

void create_monitor(Time time) {  // throws NonPositiveTime
  Monitor m(time);
}

TEST(Monitor, create) {
  auto mon = new Monitor(1_s);
  EXPECT_NE(nullptr, mon);
  delete mon;

  EXPECT_THROW(create_monitor(-1_s), NonPositiveTime);
  EXPECT_THROW(create_monitor(0_s), NonPositiveTime);
}

TEST(Monitor, createFactory) {
  auto mon = ObjectFactory::create("Monitor");
  EXPECT_EQ(nullptr, mon);
  delete mon;
}

TEST(Monitor, value) {
  Time tickold = 13.2_min;
  Time tick = tickold;

  Monitor m(tick);
  EXPECT_EQ(tickold, m.get_tick());

  tick = 3_s;
  EXPECT_EQ(tickold, m.get_tick());
}

TEST(Monitor, monitor) {
  auto obj = new GoioObj("", CmpType::HULL);
  Time t = 0_s;

  Monitor m(1_s);
  EXPECT_EQ(DmgState::NONE, m.monitor(obj, t));

  Monitor m2(1666.2_s);
  EXPECT_EQ(DmgState::NONE, m2.monitor(obj, t));

  delete obj;
}

TEST(Monitor, timefunc) {
  Monitor m(1_s);
  Time t = 0_s;
  bool b;

  auto obj1 = new GoioObj("", CmpType::BALLOON);
  EXPECT_NE(nullptr, m.get_time_func(obj1, t, b));
  delete obj1;

  auto obj2 = new GoioObj("", CmpType::COMPONENTS, -1, 0_hp, 0_hp);
  EXPECT_EQ(nullptr, m.get_time_func(obj2, t, b));
  delete obj2;

  auto obj3 = new GoioObj("", CmpType::COMPONENTS, -1, 0_hp, 1_hp);
  obj3->get_hull()->add_health(-1_hp);
  EXPECT_EQ(nullptr, m.get_time_func(obj3, t, b));
  delete obj3;
}
