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

#ifndef DMG_TYPES_H_
#define DMG_TYPES_H_

#include <string>
#include <algorithm>


namespace goio {

enum class DmgType {
  FIRE,
  FLECHETTE,
  SHATTER,
  PIERCING,
  EXPLOSIVE,
  IMPACT
};
static const std::string DmgTypeString[] {"fire",
                                          "flechette",
                                          "shatter",
                                          "piercing",
                                          "explosive",
                                          "impact"};
inline const std::string get_dmg_type_string(DmgType val) {
  return DmgTypeString[static_cast<int>(val)];
}

inline bool get_dmg_type(const std::string& val, DmgType& dmg_type) {
  auto it = std::find(std::begin(DmgTypeString),
                      std::end(DmgTypeString),
                      val);
  if (it != std::end(DmgTypeString)) {
    dmg_type = static_cast<DmgType>(it-std::begin(DmgTypeString));
    return true;
  } else {
    return false;
  }
}

enum class CmpType {
  BALLOON,
  HULL,
  ARMOR,
  COMPONENTS
};
static const std::string CmpTypeString[] {"balloon",
                                          "hull",
                                          "armor",
                                          "component"};
inline const std::string get_cmp_type_string(CmpType val) {
  return CmpTypeString[static_cast<int>(val)];
}

inline bool get_cmp_type(const std::string& val, CmpType& cmp_type) {
  auto it = std::find(std::begin(CmpTypeString),
                      std::end(CmpTypeString),
                      val);
  if (it != std::end(CmpTypeString)) {
    cmp_type = static_cast<CmpType>(it-std::begin(CmpTypeString));
    return true;
  } else {
    return false;
  }
}

static const double dmg_types[6][4] = {
  // balloon  hull  armor  components
    {  1.5,   1.3,   0.8,  0.25  },  // fire
    {  1.8,   0.2,   0.25, 0     },  // flechette
    {  0.2,   0.1,   0.2,  2     },  // shatter
    {  0.2,   0.2,   1.5,  0.2   },  // piercing
    {  0.25,  1.4,   0.3,  0.3   },  // explosive
    {  1.8,   1.5,   0.8,  0.6   },  // impact
};

inline double get_dmg_modifier(DmgType dmg_type, CmpType cmp_type) {
  return dmg_types[static_cast<int>(dmg_type)][static_cast<int>(cmp_type)];
}

/*
 * Radius where aoe damage starts to decrease.
 */
static constexpr double aoe_radius_dmg_falloff = 0.5;
/*
 * Minimum damage reduction inside aoe radius.
 */
static constexpr double aoe_dmg_falloff_min = 0.2;

/*
 * Ignition chance to apply fire stacks with explosive gun.
 */
static constexpr double explosive_ign_chance = 0.25;
/*
 * Applied stacks if explosive ignition chance is met.
 */
static const int explosive_stacks = 1;

}  // namespace goio

#endif  // DMG_TYPES_H_
