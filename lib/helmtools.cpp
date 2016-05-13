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

#include "./helmtools.h"


namespace goio {

REGISTER_TYPE_SHORT(PhoenixClaw);

HelmTool::HelmTool(double  thrust,
                   double  angular_drag,
                   double  longitudinal_drag,
                   double  lift_force,
                   double  descent_force,
                   double  vertical_drag,
                   Health_P_Time damage,
                   DmgType dmg_type,
                   CmpType dmg_target,
                   double  target_ign_chance,
                   Time    delay_after,
                   bool    tar_cloud,
                   bool    spot,
                   bool    range) :
                   thrust(thrust),
                   angular_drag(angular_drag),
                   longitudinal_drag(longitudinal_drag),
                   lift_force(lift_force),
                   descent_force(descent_force),
                   vertical_drag(vertical_drag),
                   damage(damage),
                   dmg_type(dmg_type),
                   dmg_target(dmg_target),
                   target_ign_chance(target_ign_chance),
                   delay_after(delay_after),
                   tar_cloud(tar_cloud),
                   spot(spot),
                   range(range) {
}

}  // namespace goio
