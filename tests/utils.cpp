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

#include "./utils.h"

#include <string>

#include "gtest/gtest.h"


enum class TestEnum {
  A,
  B,
  C,
  D,
  E
};
typedef Iterator<TestEnum,
                 TestEnum::A,
                 TestEnum::E> TestEnumIterator;
typedef Iterator<TestEnum,
                 TestEnum::C,
                 TestEnum::E> TestEnum1_2Iterator;
typedef Iterator<TestEnum,
                 TestEnum::B,
                 TestEnum::A> TestEnum1_3Iterator;

enum class TestEnum2 {
  A
};
typedef Iterator<TestEnum2,
                 TestEnum2::A,
                 TestEnum2::A> TestEnum2Iterator;

enum class TestEnum3 {
  A,
  B
};
typedef Iterator<TestEnum3,
                 TestEnum3::A,
                 TestEnum3::B> TestEnum3Iterator;

TEST(Utils, EnumIter) {
  int i = 0;
  TestEnum all[] {TestEnum::A, TestEnum::B, TestEnum::C, TestEnum::D, TestEnum::E};
  for (TestEnum e : TestEnumIterator()) {
    EXPECT_EQ(all[i], e);
    ++i;
  }

  int i2 = 0;
  TestEnum2 all2[] {TestEnum2::A};
  for (TestEnum2 e : TestEnum2Iterator()) {
    EXPECT_EQ(all2[i2], e);
    ++i2;
  }

  int i3 = 0;
  TestEnum3 all3[] {TestEnum3::A, TestEnum3::B};
  for (TestEnum3 e : TestEnum3Iterator()) {
    EXPECT_EQ(all3[i3], e);
    ++i3;
  }
}

TEST(Utils, EnumIterPartial) {
  int i = 0;
  TestEnum all[] {TestEnum::C, TestEnum::D, TestEnum::E};
  for (TestEnum e : TestEnum1_2Iterator()) {
    EXPECT_EQ(all[i], e);
    ++i;
  }

  // has to fail to compile
  // auto e_failed = TestEnum1_3Iterator();
}

#define VAR_MACRO(...) VFUNC(VAR_MACRO, __VA_ARGS__)
#define VAR_MACRO1(a1) a1
#define VAR_MACRO3(a1,a2,a3) a1 + a2 + a3
#define VAR_MACRO9(a1,a2,a3,a4,a5,a6,a7,a8,a9) \
  VAR_MACRO10(a1,a2,a3,a4,a5,a6,a7,a8,a9,10)
#define VAR_MACRO10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
  a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10

TEST(Utils, VariadicMacro) {
  EXPECT_EQ(1, VAR_MACRO(1));
  EXPECT_EQ(6, VAR_MACRO(1, 2, 3));
  EXPECT_EQ(55, VAR_MACRO(1, 2, 3, 4, 5, 6, 7, 8, 9));
  EXPECT_EQ(55, VAR_MACRO(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
}
