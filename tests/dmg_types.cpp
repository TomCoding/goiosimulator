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

#include "./dmg_types.h"


using namespace goio;

TEST(DmgTypes, valuesDmgType) {
  DmgType dmg_type;
  bool ret;

  {
  int i = 0;
  for (DmgType type : DmgTypeIterator()) {
    EXPECT_EQ(DmgTypeString[i], get_dmg_type_string(type));

    ret = get_dmg_type(DmgTypeString[i], dmg_type);
    EXPECT_EQ(true, ret);
    EXPECT_EQ(type, dmg_type);
    ++i;
  }
  }

  ret = get_dmg_type("", dmg_type);
  EXPECT_EQ(false, ret);
}

TEST(DmgTypes, valuesCmpType) {
  CmpType cmp_type;
  bool ret;

  {
  int i = 0;
  for (CmpType type : CmpTypeIterator()) {
    EXPECT_EQ(CmpTypeString[i], get_cmp_type_string(type));

    ret = get_cmp_type(CmpTypeString[i], cmp_type);
    EXPECT_EQ(true, ret);
    EXPECT_EQ(type, cmp_type);
    ++i;
  }
  }

  ret = get_cmp_type("", cmp_type);
  EXPECT_EQ(false, ret);
}

TEST(DmgTypes, valuesDmgMatrix) {
  EXPECT_EQ(0, get_dmg_modifier(DmgType::FLECHETTE, CmpType::COMPONENTS));

  EXPECT_EQ(dmg_types[3][2], get_dmg_modifier(DmgType::PIERCING,
                                              CmpType::ARMOR));
  EXPECT_EQ(dmg_types[4][0], get_dmg_modifier(DmgType::EXPLOSIVE,
                                              CmpType::BALLOON));
  EXPECT_EQ(dmg_types[2][3], get_dmg_modifier(DmgType::SHATTER,
                                              CmpType::COMPONENTS));
}
