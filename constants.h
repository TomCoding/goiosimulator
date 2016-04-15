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
  NONE             = 0,        // nothing changed

  TARGET           =  1 <<  0,  // damaged/healed target
  REBUILD          =  1 <<  1,  // rebuild progress on target
  IMMUNITY         =  1 <<  2,  // changed fire immunity on target
  FIRE             =  1 <<  3,  // changed fire stacks on target
  TRANSITIONED     =  1 <<  4,  // destroyed/rebuild armor/hull on target
  START_TARGET     = (1 <<  5) + (1 <<  0),
  START_REBUILD    = (1 <<  6) + (1 <<  1),
  START_IMMUNITY   = (1 <<  7) + (1 <<  2),
  START_FIRE       = (1 <<  8) + (1 <<  3),

  SELF             =  1 << 16,  // damaged/healed self
  REBUILD_S        =  1 << 17,  // rebuild progress on self
  IMMUNITY_S       =  1 << 18,  // changed fire immunity on self
  FIRE_S           =  1 << 19,  // changed fire stacks on self
  TRANSITIONED_S   =  1 << 20,  // destroyed/rebuild armor/hull on self
  START_SELF       = (1 << 21) + (1 << 16),
  START_REBUILD_S  = (1 << 22) + (1 << 17),
  START_IMMUNITY_S = (1 << 23) + (1 << 18),
  START_FIRE_S     = (1 << 24) + (1 << 19),

  SELF_ALL         = (1 << 16) +
                     (1 << 17) +
                     (1 << 18) +
                     (1 << 19) +
                     (1 << 20) +
                     (1 << 21) +
                     (1 << 22) +
                     (1 << 23) +
                     (1 << 24)
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
}

inline DmgState::State get_dmg_state_self_reversed(DmgState::State dmg_state) {
  return static_cast<DmgState::State>(
                static_cast<std::underlying_type_t<DmgState::State>>(dmg_state)
                >> 16);
}

}  // namespace goio

#endif  // CONSTANTS_H_
