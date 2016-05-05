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
  EXPECT_NE(nullptr, loch);
  delete loch;

  auto inc = new Incendiary();
  EXPECT_NE(nullptr, inc);
  delete inc;

  auto heatsink = new Heatsink();
  EXPECT_NE(nullptr, heatsink);
  delete heatsink;

  auto greased = new Greased();
  EXPECT_NE(nullptr, greased);
  delete greased;

  auto charged = new Charged();
  EXPECT_NE(nullptr, charged);
  delete charged;

  auto lesmok = new Lesmok();
  EXPECT_NE(nullptr, lesmok);
  delete lesmok;

  auto burst = new Burst();
  EXPECT_NE(nullptr, burst);
  delete burst;

  auto heavy = new Heavy();
  EXPECT_NE(nullptr, heavy);
  delete heavy;
}

TEST(Ammunitions, createFactory) {
  const int ammos_length = 8;
  std::string ammos[ammos_length] = {"Lochnagar", "Incendiary", "Heatsink",
                                     "Greased", "Charged", "Lesmok", "Burst",
                                     "Heavy"};
  for (int i = 0; i < ammos_length; ++i) {
    auto ammo = ObjectFactory::create(ammos[i]);
    EXPECT_NE(nullptr, ammo);
    EXPECT_NE(nullptr, dynamic_cast<Ammunition*>(ammo));
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
  EXPECT_EQ(0.4, ta.get_clipsize());
  EXPECT_EQ(2.25, ta.get_damage());
  EXPECT_EQ(1, ta.get_rof());
  EXPECT_EQ(0.5, ta.get_aoe_radius());
  EXPECT_EQ(0.43, ta.get_arming_time());
  EXPECT_EQ(0, ta.get_ign_chance());
  EXPECT_EQ(1.2, ta.get_projectile_speed());
  EXPECT_EQ(1.1, ta.get_shell_drop());
  EXPECT_EQ(0.45, ta.get_jitter());
  EXPECT_EQ(0.2, ta.get_turn_speed());
  EXPECT_EQ(-3, ta.get_fire_stacks());
  EXPECT_EQ(true, ta.get_proportional_self_damage());
  EXPECT_EQ(false, ta.get_immune());
}
