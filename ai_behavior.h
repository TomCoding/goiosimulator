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

#ifndef AI_BEHAVIOR_H_
#define AI_BEHAVIOR_H_


namespace goio {

enum RepairMode {
  CONSTANT_DMG_NO_WAIT,  // best repair/cooldown relation for current damage
  FASTEST_HEAL,          // get on full health as fast as possible
  // CONSTANT_DMG_WAIT   // anticipate incoming damage and repair accordingly
  // MIXED,              // fastest heal during damage pauses
};

// enum RebuildMode {
//   FASTEST_REBUILD,  // rebuild as fast as possible
//   PREREBUILD,       // only prerebuild
//   PREREBUILD_WAIT,  // prerebuild and rebuild when no more incoming damage
// };

// enum FireImmunityMode {
//   CONSTANT_IMMUNITY,  // keep component constantly immune to fire
//   NO_IMMUNITY,        // extinguish when needed
//   ANTICIPATE,         // anticipate incoming fire and maintain immunity if needed
// };

enum ExtinguishMode {
  INSTANT,        // instantly extinguish new fires
  THRESHOLD,      // only extinguish if fire damage higher than repair
  // ANTICIPATE,  // anticipate incoming fire and extinguish if over threshold
  // MIXED        // instant extinguish during damage pauses
};

}  // namespace goio

#endif  // AI_BEHAVIOR_H_
