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

#include "gtest/gtest.h"

#include <string>

#include "./ammunitions.h"
#include "./goioobj.h"


using namespace goio;

TEST(Ammunitions, create) {
  auto loch = new Lochnagar();
  EXPECT_NE(loch, nullptr);
  delete loch;

  auto inc = new Incendiary();
  EXPECT_NE(inc, nullptr);
  delete inc;

  auto heatsink = new Heatsink();
  EXPECT_NE(heatsink, nullptr);
  delete heatsink;

  auto greased = new Greased();
  EXPECT_NE(greased, nullptr);
  delete greased;

  auto charged = new Charged();
  EXPECT_NE(charged, nullptr);
  delete charged;

  auto lesmok = new Lesmok();
  EXPECT_NE(lesmok, nullptr);
  delete lesmok;

  auto burst = new Burst();
  EXPECT_NE(burst, nullptr);
  delete burst;

  auto heavy = new Heavy();
  EXPECT_NE(heavy, nullptr);
  delete heavy;
}

TEST(Ammunitions, createFactory) {
  const int ammos_length = 8;
  std::string ammos[ammos_length] = {"Lochnagar", "Incendiary", "Heatsink",
                                     "Greased", "Charged", "Lesmok", "Burst",
                                     "Heavy"};
  for (int i = 0; i < ammos_length; ++i) {
    auto ammo = ObjectFactory::create(ammos[i]);
    EXPECT_NE(ammo, nullptr);
    EXPECT_NE(dynamic_cast<Ammunition*>(ammo), nullptr);
    delete ammo;
  }
}

class TestAmmo : public Ammunition {
 public:
    TestAmmo() : Ammunition (
            0.4,           // magazine
            2.25,          // damage
            1,             // rof
            0.5,           // aoe radius
            0.43,          // arming time
            0,             // ign_chance
            1.2,           // projectile speed
            1.1,           // shell drop
            0.45,          // jitter
            0.2,           // rotation speed
            -3,            // fire stacks
            true,          // proportional self damage
            false          // temporary fire immunity
    ) {}
};

TEST(Ammunitions, values) {
  TestAmmo ta;
  EXPECT_EQ(ta.get_clipsize(), 0.4);
  EXPECT_EQ(ta.get_damage(), 2.25);
  EXPECT_EQ(ta.get_rof(), 1);
  EXPECT_EQ(ta.get_aoe_radius(), 0.5);
  EXPECT_EQ(ta.get_arming_time(), 0.43);
  EXPECT_EQ(ta.get_ign_chance(), 0);
  EXPECT_EQ(ta.get_projectile_speed(), 1.2);
  EXPECT_EQ(ta.get_shell_drop(), 1.1);
  EXPECT_EQ(ta.get_jitter(), 0.45);
  EXPECT_EQ(ta.get_turn_speed(), 0.2);
  EXPECT_EQ(ta.get_fire_stacks(), -3);
  EXPECT_EQ(ta.get_proportional_self_damage(), true);
  EXPECT_EQ(ta.get_immune(), false);
}
