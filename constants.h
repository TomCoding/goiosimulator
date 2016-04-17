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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "./dmg_types.h"
#include "./ai_behavior.h"


namespace goio {

namespace DmgState {
enum State : uint32_t {
  NONE              = 0,            // nothing changed

  TARGET            =  1ull <<  0,  // damaged/healed target
  REBUILD           =  1ull <<  1,  // rebuild progress on target
  IMMUNITY          =  1ull <<  2,  // changed fire immunity on target
  FIRE              =  1ull <<  3,  // changed fire stacks on target
  TRANSITIONED      =  1ull <<  4,  // destroyed/rebuild armor/hull on target
  START_TARGET      = (1ull <<  5) + (1ull <<  0),
  START_REBUILD     = (1ull <<  6) + (1ull <<  1),
  START_IMMUNITY    = (1ull <<  7) + (1ull <<  2),
  START_FIRE        = (1ull <<  8) + (1ull <<  3),

  START_TARGET_O    =  1ull <<  5,
  START_REBUILD_O   =  1ull <<  6,
  START_IMMUNITY_O  =  1ull <<  7,
  START_FIRE_O      =  1ull <<  8,

  TARGET_ALL        = (1ull <<  0) +
                      (1ull <<  1) +
                      (1ull <<  2) +
                      (1ull <<  3) +
                      (1ull <<  4) +
                      (1ull <<  5) +
                      (1ull <<  6) +
                      (1ull <<  7) +
                      (1ull <<  8),

  SELF              =  1ull << 16,  // damaged/healed self
  REBUILD_S         =  1ull << 17,  // rebuild progress on self
  IMMUNITY_S        =  1ull << 18,  // changed fire immunity on self
  FIRE_S            =  1ull << 19,  // changed fire stacks on self
  TRANSITIONED_S    =  1ull << 20,  // destroyed/rebuild armor/hull on self
  START_SELF        = (1ull << 21) + (1ull << 16),
  START_REBUILD_S   = (1ull << 22) + (1ull << 17),
  START_IMMUNITY_S  = (1ull << 23) + (1ull << 18),
  START_FIRE_S      = (1ull << 24) + (1ull << 19),

  START_TARGET_SO   =  1ull << 21,
  START_REBUILD_SO  =  1ull << 22,
  START_IMMUNITY_SO =  1ull << 23,
  START_FIRE_SO     =  1ull << 24,

  SELF_ALL          = (1ull << 16) +
                      (1ull << 17) +
                      (1ull << 18) +
                      (1ull << 19) +
                      (1ull << 20) +
                      (1ull << 21) +
                      (1ull << 22) +
                      (1ull << 23) +
                      (1ull << 24),

  ALL               = 0xffffffff
};

inline constexpr State operator|(State a, State b) {
  return static_cast<State>(
                static_cast<std::underlying_type_t<State>>(a) |
                static_cast<std::underlying_type_t<State>>(b));
}
inline constexpr State operator&(State a, State b) {
  return static_cast<State>(
                static_cast<std::underlying_type_t<State>>(a) &
                static_cast<std::underlying_type_t<State>>(b));
}
inline constexpr State& operator|=(State& a, State b) {
  a = a | b;
  return a;
}
}  // namespace DmgState

inline DmgState::State get_dmg_state_self_reversed(DmgState::State dmg_state) {
  return static_cast<DmgState::State>(
                static_cast<std::underlying_type_t<DmgState::State>>(dmg_state)
                >> 16);
}

}  // namespace goio

#endif  // CONSTANTS_H_
