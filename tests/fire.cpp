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

#include "./fire.h"

#include "gtest/gtest.h"
#include "./helmtools.h"


using namespace goio;

TEST(Fire, create) {
  auto fire = new Fire();
  EXPECT_NE(nullptr, fire);
  delete fire;
}

TEST(Fire, createFactory) {
  auto fire = ObjectFactory::create("Fire");
  EXPECT_EQ(nullptr, fire);
  delete fire;
}

TEST(Fire, values) {
  Fire f;
  EXPECT_EQ(0.001_s, Fire::get_firetick());
  EXPECT_EQ(CmpType::HULL, f.get_cmp_type());
  EXPECT_EQ(-1, f.get_buff_value());

  f.reset_modifiers();
  EXPECT_EQ(0.001_s, Fire::get_firetick());
  EXPECT_EQ(CmpType::HULL, f.get_cmp_type());
  EXPECT_EQ(-1, f.get_buff_value());
}

TEST(Fire, dmg) {
  auto obj1 = new FreeObject("", CmpType::BALLOON);
  obj1->set_fire(6);
  EXPECT_EQ(30_hp, Fire::get_fire_dmg(obj1, 1_s));

  EXPECT_THROW(Fire::get_fire_dmg(obj1, -1_s), NonPositiveTime);
  EXPECT_EQ(0_hp, Fire::get_fire_dmg(obj1, 0_s));
  delete obj1;

  auto obj2 = new FreeObject("", CmpType::HULL);
  obj2->set_fire(1);
  EXPECT_DOUBLE_EQ(161.2, Fire::get_fire_dmg(obj2, 12.4_s)/1_hp);
  delete obj2;

  auto obj3 = new FreeObject("", CmpType::ARMOR);
  obj3->set_fire(20);
  EXPECT_EQ(1070.976_hp, Fire::get_fire_dmg(obj3, 27.89_s));
  delete obj3;

  auto obj4 = new FreeObject("", CmpType::COMPONENTS);
  EXPECT_EQ(0_hp, Fire::get_fire_dmg(obj4, 2_s));
  delete obj4;
}

TEST(Fire, burn) {
  Fire f;
  Health max_health = 1000_hp;

  auto obj1 = new FreeObject("", CmpType::BALLOON, -1, max_health);
  obj1->set_fire(6);
  EXPECT_EQ(DmgState::TARGET, f.burn(obj1, 17.5_s));
  EXPECT_EQ(999.97_hp, obj1->get_health());
  delete obj1;

  auto obj2 = new FreeObject("", CmpType::HULL, -1, max_health);
  obj2->set_fire(1);
  EXPECT_EQ(DmgState::TARGET, f.burn(obj2, 9.3_s));
  EXPECT_EQ(999.987_hp, obj2->get_health());
  delete obj2;

  auto obj3 = new FreeObject("", CmpType::ARMOR, -1, max_health);
  obj3->set_fire(20);
  EXPECT_EQ(DmgState::TARGET, f.burn(obj3, 1_s));
  EXPECT_EQ(999.9616_hp, obj3->get_health());
  delete obj3;

  auto obj4 = new FreeObject("", CmpType::COMPONENTS, -1, max_health);
  EXPECT_EQ(DmgState::NONE, f.burn(obj4, 19_s));
  EXPECT_EQ(max_health, obj4->get_health());
  delete obj4;
}

TEST(Fire, timefunc) {
  Fire f;
  bool b;

  auto obj1 = new FreeObject("", CmpType::BALLOON);
  obj1->set_fire(6);
  EXPECT_NE(nullptr, f.get_time_func(obj1, 1_s, b));
  delete obj1;

  auto obj2 = new FreeObject("", CmpType::HULL);
  obj2->set_fire(1);
  EXPECT_NE(nullptr, f.get_time_func(obj2, 12.4_s, b));
  delete obj2;

  auto obj3 = new FreeObject("", CmpType::ARMOR);
  obj3->set_fire(20);
  EXPECT_NE(nullptr, f.get_time_func(obj3, 27.89_s, b));
  delete obj3;

  auto obj4 = new FreeObject("", CmpType::COMPONENTS);
  EXPECT_EQ(nullptr, f.get_time_func(obj4, 2_s, b));
  delete obj4;
}

TEST(Asserts, Fire) {
  Fire f;
  const HelmTool* h = new Moonshine();
  EXPECT_DEATH(f.apply_tool(h), ".*Assertion.*|");
}
