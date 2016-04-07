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

#include "./config.h"

#include <libconfig.h++>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>


namespace goio {


bool Config::find_unknown_setting(const std::set<std::string>& settings,
                                  const libconfig::Setting& settingsobj) {
  bool unknown = false;
  for (auto i = 0; i < settingsobj.getLength(); ++i) {
    if (settings.find(settingsobj[i].getName()) == settings.end()) {
      std::cerr << "Unknown setting in '" << settingsobj.getPath() << "': "
                << settingsobj[i].getName() << std::endl;
      unknown = true;
    }
  }
  return unknown;
}

int Config::load_config() {
  using libconfig::Config;
  using libconfig::FileIOException;
  using libconfig::ParseException;
  using libconfig::SettingNotFoundException;
  using libconfig::SettingTypeException;
  using libconfig::Setting;

  Config cfg;
  // Config cfg_new;

  try {
    cfg.readFile(filename.c_str());
  } catch (const FileIOException& fioex) {
    std::cerr << "I/O error while reading file." << std::endl;
    return 1;
  }
  catch (const ParseException& pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    return 1;
  }

  const Setting& root = cfg.getRoot();
  Setting& root_new = cfg_new->getRoot();

  std::set<std::string> toplevel_settings;

  const char* cur_setting = "version";
  toplevel_settings.insert(cur_setting);

  try {
    double version = root[cur_setting];
    if (version < MIN_COMP_CONFIG_VERSION) {
      std::cout << "Configuration file version not compatible: "
                << std::fixed << std::setprecision(1)
                << version << " < " << MIN_COMP_CONFIG_VERSION
                << std::endl;
      return 1;
    } else {
      root_new.add(cur_setting, Setting::TypeFloat) = version;
    }

    cur_setting = "simulations";
    toplevel_settings.insert(cur_setting);
    root_new.add(cur_setting, Setting::TypeList);
    const Setting& simulations = root[cur_setting];
    Setting& sims_new = root_new[cur_setting];

    std::set<std::string> simulation_settings;
    for (auto i = 0; i < simulations.getLength(); ++i) {
      const Setting& simulation = simulations[i];
      Setting& sim_new = sims_new.add(Setting::TypeGroup);

      cur_setting = "name";
      simulation_settings.insert(cur_setting);
      const std::string name = simulation[cur_setting];
      sim_new.add(cur_setting, Setting::TypeString) = name;

      cur_setting = "objects";
      simulation_settings.insert(cur_setting);
      sim_new.add(cur_setting, Setting::TypeList);
      const Setting& objects = simulation[cur_setting];
      Setting& objs_new = sim_new[cur_setting];

      std::set<std::string> object_settings;
      for (auto j = 0; j < objects.getLength(); ++j) {
        const Setting& object = objects[j];
        Setting& obj_new = objs_new.add(Setting::TypeGroup);

        cur_setting = "name";
        object_settings.insert(cur_setting);
        const std::string obj_name = object[cur_setting];
        obj_new.add(cur_setting, Setting::TypeString) = obj_name;

        cur_setting = "type";
        object_settings.insert(cur_setting);
        const std::string obj_type = object[cur_setting];
        obj_new.add(cur_setting, Setting::TypeString) = obj_type;

        cur_setting = "health";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          double obj_health = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_health;
        }

        cur_setting = "hull_health";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          double obj_hull_health = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_hull_health;
        }

        cur_setting = "fire";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          int obj_fire = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeInt) = obj_fire;
        }

        cur_setting = "rebuild";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          int obj_rebuild = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeInt) = obj_rebuild;
        }                         

        cur_setting = "cooldown";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          double obj_cooldown = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_cooldown;
        }                         

        cur_setting = "fire_immunity";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          double obj_fire_immunity = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_fire_immunity;
        }                         

        if (find_unknown_setting(object_settings, object))
          return 1;
      }

      cur_setting = "actors";
      simulation_settings.insert(cur_setting);
      sim_new.add(cur_setting, Setting::TypeList);
      const Setting& actors = simulation[cur_setting];
      Setting& acts_new = sim_new[cur_setting];

      std::set<std::string> actor_settings;
      for (auto j = 0; j < actors.getLength(); ++j) {
        const Setting& actor = actors[j];
        Setting& act_new = acts_new.add(Setting::TypeGroup);

        cur_setting = "name";
        actor_settings.insert(cur_setting);
        const std::string act_name = actor[cur_setting];
        act_new.add(cur_setting, Setting::TypeString) = act_name;

        cur_setting = "recipient";
        actor_settings.insert(cur_setting);
        const std::string act_recipient = actor[cur_setting];
        act_new.add(cur_setting, Setting::TypeString) = act_recipient;

        cur_setting = "action";
        actor_settings.insert(cur_setting);
        const std::string act_action = actor[cur_setting];
        act_new.add(cur_setting, Setting::TypeString) = act_action;

        cur_setting = "start";
        actor_settings.insert(cur_setting);
        if (actor.exists(cur_setting)) {
          double act_start = actor[cur_setting];
          act_new.add(cur_setting, Setting::TypeFloat) = act_start;
        }

        cur_setting = "end";
        actor_settings.insert(cur_setting);
        if (actor.exists(cur_setting)) {
          double act_end = actor[cur_setting];
          act_new.add(cur_setting, Setting::TypeFloat) = act_end;
        }

        if (find_unknown_setting(actor_settings, actor))
          return 1;
      }

      cur_setting = "options";
      simulation_settings.insert(cur_setting);
      if (simulation.exists(cur_setting)) {
        sim_new.add(cur_setting, Setting::TypeGroup);
        const Setting& options = simulation[cur_setting];
        Setting& opt_new = sim_new[cur_setting];

        std::set<std::string> option_settings;

        cur_setting = "max_events";
        option_settings.insert(cur_setting);
        if (options.exists(cur_setting)) {
          int opt_max_events = options[cur_setting];
          opt_new.add(cur_setting, Setting::TypeInt) = opt_max_events;
        }                         

        cur_setting = "max_time";
        option_settings.insert(cur_setting);
        if (options.exists(cur_setting)) {
          double opt_max_time = options[cur_setting];
          opt_new.add(cur_setting, Setting::TypeFloat) = opt_max_time;
        }                         

        if (find_unknown_setting(option_settings, options))
          return 1;
      }

      if (find_unknown_setting(simulation_settings, simulation))
        return 1;
    }

    cur_setting = "options";
    toplevel_settings.insert(cur_setting);
    if (root.exists(cur_setting)) {
      root_new.add(cur_setting, Setting::TypeGroup);
      const Setting& options = root[cur_setting];
      Setting& opt_new = root_new[cur_setting];

      std::set<std::string> option_settings;

      cur_setting = "max_events";
      option_settings.insert(cur_setting);
      if (options.exists(cur_setting)) {
        int opt_max_events = options[cur_setting];
        opt_new.add(cur_setting, Setting::TypeInt) = opt_max_events;
      }                         

      cur_setting = "max_time";
      option_settings.insert(cur_setting);
      if (options.exists(cur_setting)) {
        double opt_max_time = options[cur_setting];
        opt_new.add(cur_setting, Setting::TypeFloat) = opt_max_time;
      }                         

      if (find_unknown_setting(option_settings, options))
        return 1;
    }

    if (find_unknown_setting(toplevel_settings, root))
      return 1;
  } catch (const SettingNotFoundException& nfex) {
    std::cerr << "No '" << nfex.getPath() << "' setting in configuration file."
              << std::endl;
  } catch (const SettingTypeException& ste) {
    std::cerr << "'" << ste.getPath() << "' setting has wrong type." << std::endl;
  }

  return 0;
}

int Config::write() {
  try {
    cfg_new->writeFile(filename.c_str());
    std::cerr << "New configuration successfully written to: " << filename
              << std::endl;
  } catch(const libconfig::FileIOException& fioex) {
    std::cerr << "I/O error while writing file: " << filename << std::endl;
    return 1;
  }
  return 0;
}

}  // namespace goio
