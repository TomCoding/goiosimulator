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

#include "./ammunitions.h"


namespace goio {

REGISTER_TYPE_SHORT(Lochnagar, "Lochnagar");
REGISTER_TYPE_SHORT(Incendiary, "Incendiary");
REGISTER_TYPE_SHORT(Heatsink, "Heatsink");
REGISTER_TYPE_SHORT(Greased, "Greased");
REGISTER_TYPE_SHORT(Charged, "Charged");
REGISTER_TYPE_SHORT(Lesmok, "Lesmok");
REGISTER_TYPE_SHORT(Burst, "Burst");
REGISTER_TYPE_SHORT(Heavy, "Heavy");

Ammunition::Ammunition(double clipsize, double damage, double rof,
                       double aoe_radius, double arming_time,
                       double ign_chance, double projectile_speed,
                       double shell_drop, double jitter, double turn_speed,
                       int fire_stacks, bool proportional_self_damage,
                       bool immune) :
                       clipsize(clipsize), damage(damage), rof(rof),
                       aoe_radius(aoe_radius), arming_time(arming_time),
                       ign_chance(ign_chance), projectile_speed(projectile_speed),
                       shell_drop(shell_drop), jitter(jitter),
                       turn_speed(turn_speed), fire_stacks(fire_stacks),
                       proportional_self_damage(proportional_self_damage),
                       immune(immune) {
}

}  // namespace goio
