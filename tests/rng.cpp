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

#include "./rng.h"

#include "gtest/gtest.h"


using namespace goio;

TEST(Rng, Randomness) {
  EXPECT_EQ(nullptr, random_percentage);
  init();
  ASSERT_NE(nullptr, random_percentage);
  for (int i = 0; i < 1000; ++i) {
    auto percent = random_percentage();
    ASSERT_NE(random_percentage(), percent);
    ASSERT_LE(0, percent);
    ASSERT_GE(1, percent);
  }
}
