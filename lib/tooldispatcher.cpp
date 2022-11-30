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

#include "./tooldispatcher.h"

#include "./helmtools.h"
#include "./ammunitions.h"
#include "./repairtools.h"

#include "./ships.h"
#include "./engines.h"
#include "./balloon.h"
#include "./guns.h"


namespace goio {

void ToolDispatcher::apply_tool(Ship* ship, const HelmTool* tool, bool activate) {
  if (ship == nullptr || tool == nullptr)
    return;

  int fac = (activate) ? 1 : -1;
  ship->add_angular_drag_mod(fac*tool->get_angular_drag());
  ship->add_longitudinal_drag_mod(fac*tool->get_longitudinal_drag());
  ship->add_vertical_drag_mod(fac*tool->get_vertical_drag());

  for (auto engine : ship->get_light_engines())
    if (activate)
      engine->apply_tool(tool);
    else
      engine->remove_tool(tool);

  for (auto engine : ship->get_heavy_engines())
    if (activate)
      engine->apply_tool(tool);
    else
      engine->remove_tool(tool);


  if (activate)
    ship->get_balloon()->apply_tool(tool);
  else
    ship->get_balloon()->remove_tool(tool);
}

void ToolDispatcher::apply_tool(Engine* engine, const HelmTool* tool, bool activate) {
  if (engine == nullptr || tool == nullptr)
    return;

  int fac = (activate) ? 1 : -1;
  engine->add_thrust_mod(fac*tool->get_thrust());
}

void ToolDispatcher::apply_tool(Balloon* balloon, const HelmTool* tool, bool activate) {
  if (balloon == nullptr || tool == nullptr)
    return;

  int fac = (activate) ? 1 : -1;
  balloon->add_lift_force_mod(fac*tool->get_lift_force());
  balloon->add_descent_force_mod(fac*tool->get_descent_force());
}


void ToolDispatcher::apply_tool(Gun* gun, const Ammunition* tool, bool activate) {
  if (gun == nullptr || tool == nullptr)
    return;

  int fac = (activate) ? 1 : -1;
  gun->add_clipsize_mod(fac*tool->get_clipsize());
  gun->add_direct_dmg_mod(fac*tool->get_damage());
  gun->add_aoe_dmg_mod(fac*tool->get_damage());
  gun->add_rof_mod(fac*tool->get_rof());
  gun->add_aoe_radius_mod(fac*tool->get_aoe_radius());
  gun->add_arming_time_mod(fac*tool->get_arming_time());
  gun->add_direct_ign_chance_mod(fac*tool->get_ign_chance());
  gun->add_aoe_ign_chance_mod(fac*tool->get_ign_chance());
  gun->add_projectile_speed_mod(fac*tool->get_projectile_speed());
  gun->add_shell_drop_mod(fac*tool->get_shell_drop());
  gun->add_jitter_mod(fac*tool->get_jitter());
  gun->add_turn_left_mod(fac*tool->get_turn_speed());
  gun->add_turn_right_mod(fac*tool->get_turn_speed());
  gun->add_turn_up_mod(fac*tool->get_turn_speed());
  gun->add_turn_down_mod(fac*tool->get_turn_speed());
  gun->add_turn_horizontal_mod(fac*tool->get_turn_speed());
  gun->add_turn_vertical_mod(fac*tool->get_turn_speed());

  gun->add_fire(fac*tool->get_fire_stacks());
  gun->set_temporary_immunity(fac*tool->get_immune());
}


void ToolDispatcher::apply_tool(Ship* ship, const BuffTool* tool, bool activate) {
  if (ship == nullptr || tool == nullptr)
    return;
  if (activate)
    ship->apply_tool(&(tool->armor_buff));
  else
    ship->remove_tool(&(tool->armor_buff));
}

void ToolDispatcher::apply_tool(Engine* engine, const BuffTool* tool, bool activate) {
  if (engine == nullptr || tool == nullptr)
    return;
  if (activate)
    engine->apply_tool(&(tool->engines_buff));
  else
    engine->remove_tool(&(tool->engines_buff));
}

void ToolDispatcher::apply_tool(Balloon* balloon, const BuffTool* tool, bool activate) {
  if (balloon == nullptr || tool == nullptr)
    return;
  if (activate)
    balloon->apply_tool(&(tool->engines_buff));
  else
    balloon->remove_tool(&(tool->engines_buff));
}

void ToolDispatcher::apply_tool(Gun* gun, const BuffTool* tool, bool activate) {
  if (gun == nullptr || tool == nullptr)
    return;
  if (activate)
    gun->apply_tool(&(tool->guns_buff));
  else
    gun->remove_tool(&(tool->guns_buff));
}

}  // namespace goio
