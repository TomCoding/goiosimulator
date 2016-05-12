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

#include "./ai_behavior.h"

#include <string>

#include "gtest/gtest.h"


using namespace goio;

TEST(AIBehavior, valuesRepairMode) {
  RepairMode repair_mode;
  bool ret;

  {
  int i = 0;
  for (RepairMode mode : RepairModeIterator()) {
    EXPECT_EQ(RepairModeString[i], get_repair_mode_string(mode));

    ret = get_repair_mode(RepairModeString[i], repair_mode);
    EXPECT_TRUE(ret);
    EXPECT_EQ(mode, repair_mode);
    ++i;
  }
  }

  ret = get_repair_mode("", repair_mode);
  EXPECT_FALSE(ret);
}

TEST(AIBehavior, valuesExtinguishMode) {
  ExtinguishMode ext_mode;
  bool ret;

  {
  int i = 0;
  for (ExtinguishMode mode : ExtinguishModeIterator()) {
    EXPECT_EQ(ExtinguishModeString[i], get_extinguish_mode_string(mode));

    ret = get_extinguish_mode(ExtinguishModeString[i], ext_mode);
    EXPECT_TRUE(ret);
    EXPECT_EQ(mode, ext_mode);
    ++i;
  }
  }

  ret = get_extinguish_mode("", ext_mode);
  EXPECT_FALSE(ret);
}
