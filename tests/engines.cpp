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

#include "./engines.h"

#include <string>

#include "gtest/gtest.h"


using namespace goio;

void test_const_values(const Engine& e, const std::string& name, Thrust thrust);
void test_variable_values(const Engine& e, Thrust thrust, bool all = true);
void test_changed_health(Engine& e, const std::string& name,
                         Thrust thrust, Thrust thrust_new,
                         Health max_health, Health added_health);
double get_thrust_mod(Thrust old_thrust, Thrust new_thrust);

TEST(Engines, create) {
  std::string name = "test engines";
  Thrust thrust = 120000_N;

  auto le = new LightEngine(name, thrust);
  EXPECT_NE(nullptr, le);
  delete le;

  auto he = new HeavyEngine(name, thrust);
  EXPECT_NE(nullptr, he);
  delete he;
}

TEST(Engines, createFactory) {
  auto le = ObjectFactory::create("LightEngine");
  EXPECT_EQ(nullptr, le);
  delete le;

  auto he = ObjectFactory::create("HeavyEngine");
  EXPECT_EQ(nullptr, he);
  delete he;
}

TEST(Engines, GoioObj) {
  LightEngine le("", 0_N);
  EXPECT_EQ(40, le.get_rebuild_value());

  HeavyEngine he("", 0_N);
  EXPECT_EQ(59, he.get_rebuild_value());
}

void test_const_values(const Engine& e, const std::string& name, Thrust thrust) {
  EXPECT_EQ(name, e.get_name());
  EXPECT_EQ(thrust, e.get_orig_thrust());
}
void test_variable_values(const Engine& e, Thrust thrust, bool all) {
  EXPECT_FLOAT_EQ(thrust/1_N, e.get_thrust()/1_N);
  if (all)
    EXPECT_FLOAT_EQ(thrust/1_N, e.get_thrust_changed()/1_N);
}

TEST(Engines, values) {
  std::string name = "";
  Thrust thrust = 3992.3499_N;

  LightEngine le(name, thrust);
  test_const_values(le, name, thrust);
  test_variable_values(le, thrust);

  HeavyEngine he(name, thrust);
  test_const_values(he, name, thrust);
  test_variable_values(he, thrust);
}

double get_thrust_mod(Thrust old_thrust, Thrust new_thrust) {
  return new_thrust/old_thrust - 1;
}

TEST(Engines, valuesSet) {
  std::string name = "valuesSet";
  Thrust thrust = 3399002_N;

  Thrust thrust_new = 2001992.47_N;
  double thrust_mod = get_thrust_mod(thrust, thrust_new);

  LightEngine le(name, thrust);
  le.add_thrust_mod(thrust_mod);
  test_const_values(le, name, thrust);
  test_variable_values(le, thrust_new);

  le.add_thrust_mod(-thrust_mod);
  test_const_values(le, name, thrust);
  test_variable_values(le, thrust);

  le.add_thrust_mod(-200);
  test_const_values(le, name, thrust);
  test_variable_values(le, 0_N);

  HeavyEngine he(name, thrust);
  he.add_thrust_mod(thrust_mod);
  test_const_values(he, name, thrust);
  test_variable_values(he, thrust_new);

  he.add_thrust_mod(-thrust_mod);
  test_const_values(he, name, thrust);
  test_variable_values(he, thrust);

  he.add_thrust_mod(-200);
  test_const_values(he, name, thrust);
  test_variable_values(he, 0_N);
}

void test_changed_health(Engine& e, const std::string& name,
                         Thrust thrust, Thrust thrust_new,
                         Health max_health, Health added_health) {
  e.add_thrust_mod(get_thrust_mod(thrust, thrust_new));
  e.add_health(added_health);
  EXPECT_EQ(max_health, e.get_max_health());
  test_const_values(e, name, thrust);
  test_variable_values(e, thrust_new, false);
  EXPECT_EQ(max_health+added_health, e.get_health());

  auto health_mod = e.get_health()/max_health;
  EXPECT_FLOAT_EQ(health_mod*thrust_new/1_N, e.get_thrust_changed()/1_N);
}

TEST(Engines, healthSet) {
  std::string name = "healthSet";
  Thrust thrust = 768220_N;

  Thrust thrust_new = 345008_N;
  Health added_health = -203_hp;

  LightEngine le(name, thrust);
  test_changed_health(le, name, thrust, thrust_new, 300_hp, added_health);

  HeavyEngine he(name, thrust);
  test_changed_health(he, name, thrust, thrust_new, 525_hp, added_health);
}

TEST(Engines, reset) {
  std::string name = "reset";
  Thrust thrust = 288400774_N;

  Thrust thrust_new = 38484440048_N;
  double thrust_mod = get_thrust_mod(thrust, thrust_new);

  LightEngine le(name, thrust);
  le.add_thrust_mod(thrust_mod);
  test_const_values(le, name, thrust);
  test_variable_values(le, thrust_new);
  le.reset_modifiers();
  test_const_values(le, name, thrust);
  test_variable_values(le, thrust);

  HeavyEngine he(name, thrust);
  he.add_thrust_mod(thrust_mod);
  test_const_values(he, name, thrust);
  test_variable_values(he, thrust_new);
  he.reset_modifiers();
  test_const_values(he, name, thrust);
  test_variable_values(he, thrust);
}
