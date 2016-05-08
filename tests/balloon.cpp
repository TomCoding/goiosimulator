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

#include "./balloon.h"


using namespace goio;

void test_const_values(Balloon& b, std::string& name, Force lift_force,
                       Health max_health);
void test_variable_values(Balloon& b, Force lift_force, Force descent_force,
                       Health max_health, bool all = true);

TEST(Balloon, create) {
  std::string name = "test balloon";
  Force lift_force = 100000_N;
  Health max_health = 200_hp;

  auto balloon1 = new Balloon(name, lift_force);
  EXPECT_NE(nullptr, balloon1);
  delete balloon1;

  auto balloon2 = new Balloon(name, lift_force, max_health);
  EXPECT_NE(nullptr, balloon2);
  delete balloon2;
}

TEST(Balloon, createFactory) {
  auto balloon = ObjectFactory::create("Balloon");
  EXPECT_EQ(nullptr, balloon);
  delete balloon;
}

TEST(Balloon, GoioObj) {
  Balloon b("", 0_N, 1200_hp);
  EXPECT_EQ(46, b.get_rebuild_value());
}

void test_const_values(Balloon& b, std::string& name, Force lift_force,
                       Health max_health) {
  EXPECT_EQ(name, b.get_name());
  EXPECT_EQ(max_health, b.get_max_health());
  EXPECT_EQ(lift_force, b.get_orig_lift_force());
  EXPECT_EQ(lift_force, b.get_orig_descent_force());
}
void test_variable_values(Balloon& b, Force lift_force, Force descent_force,
                          Health max_health, bool all) {
  EXPECT_EQ(lift_force, b.get_lift_force());
  EXPECT_EQ(descent_force, b.get_descent_force());
  if (all) {
    EXPECT_EQ(max_health, b.get_health());
    EXPECT_EQ(lift_force, b.get_lift_force_changed());
    EXPECT_EQ(descent_force, b.get_descent_force_changed());
  }
}

TEST(Balloon, values) {
  std::string name = "";
  Force lift_force = 503030.34_N;
  Health max_health = 1750_hp;

  Balloon b(name, lift_force, max_health);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force, lift_force, max_health);
}

TEST(Balloon, valuesSet) {
  std::string name = "valuesSet";
  Force lift_force = 778293_N;
  Health max_health = 50_hp;

  Force lift_force_new = 500103_N;
  Force descent_force_new = 192775.3_N;

  Balloon b(name, lift_force, max_health);
  b.set_lift_force(lift_force_new);
  b.set_descent_force(descent_force_new);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health);

  b.set_lift_force(-lift_force_new);
  b.set_descent_force(-descent_force_new);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, 0_N, 0_N, max_health);
}

TEST(Balloon, healthSet) {
  std::string name = "healthSet";
  Force lift_force = 992840_N;
  Health max_health = 3350_hp;

  Force lift_force_new = 1000827_N;
  Force descent_force_new = 41_N;
  Health added_health = -789_hp;

  Balloon b(name, lift_force, max_health);
  b.set_lift_force(lift_force_new);
  b.set_descent_force(descent_force_new);
  b.add_health(added_health);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health, false);
  EXPECT_EQ(max_health+added_health, b.get_health());

  auto health_mod = b.get_health()/max_health;
  EXPECT_EQ(health_mod*lift_force_new, b.get_lift_force_changed());
  EXPECT_EQ(health_mod*descent_force_new, b.get_descent_force_changed());
}

TEST(Balloon, reset) {
  std::string name = "reset";
  Force lift_force = 2840_N;
  Health max_health = 357.299_hp;

  Force lift_force_new = 1827_N;
  Force descent_force_new = 4100_N;

  Balloon b(name, lift_force, max_health);
  b.set_lift_force(lift_force_new);
  b.set_descent_force(descent_force_new);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health);

  b.reset(true);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force, lift_force, max_health);
}
