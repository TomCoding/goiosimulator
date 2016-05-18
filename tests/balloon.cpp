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

#include "./balloon.h"

#include <string>

#include "gtest/gtest.h"


using namespace goio;

void create_balloon(Health health);
void test_const_values(const Balloon& b, const std::string& name, Force lift_force,
                       Health max_health);
void test_variable_values(const Balloon& b, Force lift_force, Force descent_force,
                       Health max_health, bool all = true);
double get_force_mod(Force old_force, Force new_force);

void create_balloon(Health health) {  // throws NonPositiveHealth
  Balloon b("", 100_N, health);
}

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

  EXPECT_THROW(create_balloon(-1_hp), NonPositiveHealth);
  EXPECT_THROW(create_balloon(0_hp), NonPositiveHealth);

  auto mb = new MineBalloon(name);
  EXPECT_NE(nullptr, mb);
  if (mb != nullptr) {
    EXPECT_EQ(0_N, mb->get_lift_force());
    EXPECT_EQ(200_hp, mb->get_health());
    EXPECT_EQ(200_hp, mb->get_max_health());
  }
  delete mb;
}

TEST(Balloon, createFactory) {
  auto balloon = ObjectFactory::create("Balloon");
  EXPECT_EQ(nullptr, balloon);
  delete balloon;

  auto mb = ObjectFactory::create("MineBalloon");
  EXPECT_EQ(nullptr, mb);
  delete mb;
}

TEST(Balloon, GoioObj) {
  Balloon b("", 0_N, 1200_hp);
  EXPECT_EQ(46, b.get_rebuild_value());
}

void test_const_values(const Balloon& b, const std::string& name, Force lift_force,
                       Health max_health) {
  EXPECT_EQ(name, b.get_name());
  EXPECT_EQ(max_health, b.get_max_health());
  EXPECT_EQ(lift_force, b.get_orig_lift_force());
  EXPECT_EQ(lift_force, b.get_orig_descent_force());
}
void test_variable_values(const Balloon& b, Force lift_force, Force descent_force,
                          Health max_health, bool all) {
  EXPECT_FLOAT_EQ(lift_force/1_N, b.get_lift_force()/1_N);
  EXPECT_FLOAT_EQ(descent_force/1_N, b.get_descent_force()/1_N);
  if (all) {
    EXPECT_EQ(max_health, b.get_health());
    EXPECT_FLOAT_EQ(lift_force/1_N, b.get_lift_force_changed()/1_N);
    EXPECT_FLOAT_EQ(descent_force/1_N, b.get_descent_force_changed()/1_N);
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

double get_force_mod(Force old_force, Force new_force) {
  return new_force/old_force - 1;
}

TEST(Balloon, valuesSet) {
  std::string name = "valuesSet";
  Force lift_force = 778293_N;
  Health max_health = 50_hp;

  Force lift_force_new = 500103_N;
  double lift_mod = get_force_mod(lift_force, lift_force_new);
  Force descent_force_new = 192775.3_N;
  double descent_mod = get_force_mod(lift_force, descent_force_new);

  Balloon b(name, lift_force, max_health);
  b.add_lift_force_mod(lift_mod);
  b.add_descent_force_mod(descent_mod);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health);

  b.add_lift_force_mod(-lift_mod);
  test_variable_values(b, lift_force, descent_force_new, max_health);
  b.add_descent_force_mod(-descent_mod);
  test_variable_values(b, lift_force, lift_force, max_health);

  b.add_lift_force_mod(-200);
  b.add_descent_force_mod(-200);
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
  b.add_lift_force_mod(get_force_mod(lift_force, lift_force_new));
  b.add_descent_force_mod(get_force_mod(lift_force, descent_force_new));
  b.add_health(added_health);
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health, false);
  EXPECT_EQ(max_health+added_health, b.get_health());

  auto health_mod = b.get_health()/max_health;
  EXPECT_DOUBLE_EQ(health_mod*lift_force_new/1_N, b.get_lift_force_changed()/1_N);
  EXPECT_FLOAT_EQ(health_mod*descent_force_new/1_N,
                  b.get_descent_force_changed()/1_N);
}

TEST(Balloon, reset) {
  std::string name = "reset";
  Force lift_force = 2840_N;
  Health max_health = 357.299_hp;

  Force lift_force_new = 1827_N;
  Force descent_force_new = 4100_N;

  Balloon b(name, lift_force, max_health);
  b.add_lift_force_mod(get_force_mod(lift_force, lift_force_new));
  b.add_descent_force_mod(get_force_mod(lift_force, descent_force_new));
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force_new, descent_force_new, max_health);

  b.reset_modifiers();
  test_const_values(b, name, lift_force, max_health);
  test_variable_values(b, lift_force, lift_force, max_health);
}
