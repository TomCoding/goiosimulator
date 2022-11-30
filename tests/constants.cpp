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

#include "./constants.h"

#include <string>
#include <sstream>
#include <exception>

#include "gtest/gtest.h"


using namespace goio;

TEST(Constants, DmgStateOR) {
  EXPECT_EQ((1ull << 16) + (1ull << 19), DmgState::SELF | DmgState::FIRE_S);
  EXPECT_EQ((1ull << 2) + (1ull << 6) + (1ull << 18),
            DmgState::IMMUNITY | DmgState::TRANSITIONED | DmgState::IMMUNITY_S);

  EXPECT_EQ(DmgState::START_TARGET, DmgState::TARGET | DmgState::START_TARGET_O);
  EXPECT_EQ(DmgState::START_REBUILD, DmgState::REBUILD | DmgState::START_REBUILD_O);
  EXPECT_EQ(DmgState::START_IMMUNITY,
            DmgState::IMMUNITY | DmgState::START_IMMUNITY_O);
  EXPECT_EQ(DmgState::START_FIRE, DmgState::FIRE | DmgState::START_FIRE_O);

  EXPECT_EQ(DmgState::START_SELF, DmgState::SELF | DmgState::START_TARGET_SO);
  EXPECT_EQ(DmgState::START_REBUILD_S,
            DmgState::REBUILD_S | DmgState::START_REBUILD_SO);
  EXPECT_EQ(DmgState::START_IMMUNITY_S,
            DmgState::IMMUNITY_S | DmgState::START_IMMUNITY_SO);
  EXPECT_EQ(DmgState::START_FIRE_S, DmgState::FIRE_S | DmgState::START_FIRE_SO);

  EXPECT_EQ(DmgState::TARGET_ALL,
            DmgState::START_TARGET | DmgState::START_REBUILD |
            DmgState::START_IMMUNITY | DmgState::START_FIRE |
            DmgState::START_BUFF | DmgState::START_PREBUFF |
            DmgState::TRANSITIONED);
  EXPECT_EQ(DmgState::SELF_ALL,
            DmgState::START_SELF | DmgState::START_REBUILD_S |
            DmgState::START_IMMUNITY_S | DmgState::START_FIRE_S |
            DmgState::START_BUFF_S | DmgState::START_PREBUFF_S |
            DmgState::TRANSITIONED_S);
}

TEST(Constants, DmgStateAND) {
  EXPECT_EQ(DmgState::FIRE, DmgState::TARGET_ALL & DmgState::FIRE);
  EXPECT_EQ(DmgState::NONE, DmgState::SELF_ALL & DmgState::TRANSITIONED);
  EXPECT_EQ(DmgState::TARGET,
            DmgState::ALL & DmgState::START_TARGET & DmgState::TARGET);
  EXPECT_EQ(DmgState::NONE,
            DmgState::START_FIRE_S & DmgState::START_FIRE_SO & DmgState::FIRE_S);

  EXPECT_EQ(DmgState::NONE, DmgState::TARGET & DmgState::START_TARGET_O);
  EXPECT_EQ(DmgState::NONE, DmgState::REBUILD & DmgState::START_REBUILD_O);
  EXPECT_EQ(DmgState::NONE, DmgState::IMMUNITY & DmgState::START_IMMUNITY_O);
  EXPECT_EQ(DmgState::NONE, DmgState::FIRE & DmgState::START_FIRE_O);

  EXPECT_EQ(DmgState::NONE, DmgState::SELF & DmgState::START_TARGET_SO);
  EXPECT_EQ(DmgState::NONE, DmgState::REBUILD_S & DmgState::START_REBUILD_SO);
  EXPECT_EQ(DmgState::NONE, DmgState::IMMUNITY_S & DmgState::START_IMMUNITY_SO);
  EXPECT_EQ(DmgState::NONE, DmgState::FIRE_S & DmgState::START_FIRE_SO);
}

TEST(Constants, DmgStateAssignmentOR) {
  DmgState dmg_state = DmgState::SELF;
  EXPECT_EQ(DmgState::START_SELF, dmg_state |= DmgState::START_TARGET_SO);

  dmg_state = DmgState::REBUILD_S;
  EXPECT_EQ(DmgState::START_REBUILD_S, dmg_state |= DmgState::START_REBUILD_SO);

  dmg_state = DmgState::FIRE_S;
  EXPECT_EQ(DmgState::START_FIRE_S, dmg_state |= DmgState::START_FIRE_SO);

  dmg_state = DmgState::IMMUNITY_S;
  EXPECT_EQ(DmgState::START_IMMUNITY_S, dmg_state |= DmgState::START_IMMUNITY_SO);
}

TEST(Constants, DmgStateReversed) {
  EXPECT_EQ(DmgState::TARGET, get_dmg_state_self_reversed(DmgState::SELF));
  EXPECT_EQ(DmgState::REBUILD, get_dmg_state_self_reversed(DmgState::REBUILD_S));
  EXPECT_EQ(DmgState::IMMUNITY, get_dmg_state_self_reversed(DmgState::IMMUNITY_S));
  EXPECT_EQ(DmgState::FIRE, get_dmg_state_self_reversed(DmgState::FIRE_S));
  EXPECT_EQ(DmgState::BUFF, get_dmg_state_self_reversed(DmgState::BUFF_S));
  EXPECT_EQ(DmgState::PREBUFF, get_dmg_state_self_reversed(DmgState::PREBUFF_S));
  EXPECT_EQ(DmgState::TARGET_ALL, get_dmg_state_self_reversed(DmgState::SELF_ALL));
  EXPECT_EQ(DmgState::NONE, get_dmg_state_self_reversed(DmgState::TARGET_ALL));
  EXPECT_EQ(DmgState::NONE, get_dmg_state_self_reversed(DmgState::ALL) &
                            DmgState::SELF_ALL);
}

TEST(Constants, UnitValues) {
  EXPECT_EQ(3_m, Distance(3));
  EXPECT_EQ(3._m, Distance(3.));
  EXPECT_EQ(3_s, Time(3));
  EXPECT_EQ(3._s, Time(3.));
  EXPECT_EQ(1_min, Time(60));
  EXPECT_EQ(1._min, Time(60.));
  EXPECT_EQ(3/1_s, P_Time(3));
  EXPECT_EQ(3/1._s, P_Time(3.));
  EXPECT_EQ(3_hp, Health(3));
  EXPECT_EQ(3._hp, Health(3.));
  EXPECT_EQ(3_m/1_s, Speed(3));
  EXPECT_EQ(3._m/1._s, Speed(3.));
  EXPECT_EQ(3_m_s2, Acceleration(3));
  EXPECT_EQ(3._m_s2, Acceleration(3.));
  EXPECT_EQ(3_kg, Weight(3));
  EXPECT_EQ(3._kg, Weight(3.));
  EXPECT_EQ(3_N, Force(3));
  EXPECT_EQ(3._N, Force(3.));
  EXPECT_EQ(3_N, Thrust(3));
  EXPECT_EQ(3._N, Thrust(3.));
  EXPECT_EQ(3_deg, Angle(3));
  EXPECT_EQ(3._deg, Angle(3.));
  EXPECT_EQ(3_deg/1_s, Angular_Speed(3));
  EXPECT_EQ(3._deg/1._s, Angular_Speed(3.));
  EXPECT_EQ(3_deg_s2, Angular_Acceleration(3));
  EXPECT_EQ(3._deg_s2, Angular_Acceleration(3.));
}

#ifdef STRONG_TYPE_COMPILE
TEST(Constants, UnitStructs) {
  Value<Unit<1, 0, 0, 0, 0>> x;
  EXPECT_EQ(0_m, x);

  Value<Unit<1, 0, 0, 0, 0>> y(3);
  EXPECT_EQ(3_m, y);

  auto z = Value<Unit<1, 0, 0, 0, 0>>(3);
  EXPECT_EQ(3_m, z);

  auto p = 3_m;
  EXPECT_EQ(3_m, p);
}

TEST(Constants, UnitStream) {
  std::ostringstream stream;
  stream << 1_m;
  EXPECT_EQ("1", stream.str());
}

TEST(Constants, UnitCalcs) {
  EXPECT_EQ(3_m, 1_m+2_m);
  EXPECT_EQ(1_m, 3_m-2_m);

  auto d = 1_m;
  EXPECT_EQ(1_m, d);
  EXPECT_EQ(3_m, d += 2_m);
  EXPECT_EQ(3_m, d);
  EXPECT_EQ(1_m, d -= 2_m);
  EXPECT_EQ(1_m, d);

  EXPECT_NE(3_m, 1_m);
  EXPECT_GT(3_m, 1_m);
  EXPECT_LT(1_m, 3_m);
  EXPECT_GE(3_m, 3_m);
  EXPECT_GE(3_m, 1_m);
  EXPECT_LE(3_m, 3_m);
  EXPECT_LE(1_m, 3_m);

  EXPECT_EQ(-3, (-3_m).val);
  EXPECT_EQ(6_m, 2_m*3);
  EXPECT_EQ(6_m, 3*2_m);
  EXPECT_EQ(6_m, 2_m*3.0);
  EXPECT_EQ(6_m, 3.0*2_m);
  EXPECT_EQ(2_m, 6_m/3.0);
  EXPECT_EQ(2_m, 6_m/3);
  EXPECT_EQ(3/1_m, 6.0/2_m);

  EXPECT_EQ(1, static_cast<double>(3_m/3_m));

  EXPECT_EQ(9_m/1_s, 3_m_s2*3_s);
  EXPECT_EQ(3_m_s2, 3_N/1_kg);
}

TEST(Constants, Sqrt) {
  EXPECT_EQ(2_m, sqrt(4_m*1_m));
  EXPECT_EQ(4_m/1_s, sqrt(16_m_s2*1_m));
  EXPECT_EQ(4/1_m, sqrt(16/1_m/1_m));
  EXPECT_EQ(2, static_cast<double>(sqrt(4_m/1_m)));

  EXPECT_THROW(sqrt(4_m), std::invalid_argument);
  EXPECT_THROW(sqrt(4_m_s2), std::invalid_argument);
}
#endif

TEST(Constants, Regressions) {
  // operator-() assignment instead of copy
  auto dist = 1_m;
  EXPECT_EQ(-1_m, -dist);
  EXPECT_EQ(1_m, dist);
}
