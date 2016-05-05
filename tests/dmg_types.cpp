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
  const int dmg_types_count = 6;
  DmgType dmg_types[dmg_types_count] = {DmgType::FIRE, DmgType::FLECHETTE,
                                        DmgType::SHATTER, DmgType::PIERCING,
                                        DmgType::EXPLOSIVE, DmgType::IMPACT};
  std::string dmg_types_string[dmg_types_count] = {"fire", "flechette",
                                                   "shatter", "piercing",
                                                   "explosive", "impact"};
  DmgType dmg_type;
  bool ret;

  for (int i = 0; i < dmg_types_count; ++i) {
    EXPECT_EQ(dmg_types_string[i], get_dmg_type_string(dmg_types[i]));

    ret = get_dmg_type(dmg_types_string[i], dmg_type);
    EXPECT_EQ(true, ret);
    EXPECT_EQ(dmg_types[i], dmg_type);
  }

  ret = get_dmg_type("", dmg_type);
  EXPECT_EQ(false, ret);
}

TEST(DmgTypes, valuesCmpType) {
  const int cmp_types_count = 4;
  CmpType cmp_types[cmp_types_count] = {CmpType::BALLOON, CmpType::HULL,
                                        CmpType::ARMOR, CmpType::COMPONENTS};
  std::string cmp_types_string[cmp_types_count] = {"balloon", "hull",
                                                   "armor", "component"};
  CmpType cmp_type;
  bool ret;

  for (int i = 0; i < cmp_types_count; ++i) {
    EXPECT_EQ(cmp_types_string[i], get_cmp_type_string(cmp_types[i]));

    ret = get_cmp_type(cmp_types_string[i], cmp_type);
    EXPECT_EQ(true, ret);
    EXPECT_EQ(cmp_types[i], cmp_type);
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
