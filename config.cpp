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

#include <stdint.h>

#include <libconfig.h++>
#include <set>
#include <tuple>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>

#include "./goioactor.h"
#include "./guns.h"
#include "./ammunitions.h"
#include "./repairtools.h"


namespace goio {

Config::~Config() {
  for (auto it = simulations.begin(); it != simulations.end(); ++it) {
    delete std::get<1>(*it);
    delete std::get<2>(*it);
    for (auto it2 = std::get<3>(*it).begin();
                                    it2 != std::get<3>(*it).end(); ++it2) {
      delete it2->second;
    }
  }
  delete cfg_new;
}

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

  try {
    cfg.readFile(filename.c_str());
  } catch (const FileIOException& fioex) {
    std::cerr << "I/O error while reading file '" << filename << "'." << std::endl;
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


      this->simulations.push_back(
            std::make_tuple(name,
                            new TimeObj(),
                            nullptr,
                            std::unordered_map<std::string, Object*>()));
      auto& factory_objects = std::get<3>(this->simulations.back());

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

        cur_setting = "ammo";
        object_settings.insert(cur_setting);
        std::string obj_ammo = "";
        if (object.exists(cur_setting)) {
          obj_ammo = static_cast<const char*>(object[cur_setting]);
          obj_new.add(cur_setting, Setting::TypeString) = obj_ammo;
        }

        cur_setting = "health";
        object_settings.insert(cur_setting);
        double obj_health = -1;
        if (object.exists(cur_setting)) {
          obj_health = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_health;
        }

        cur_setting = "hull_health";
        object_settings.insert(cur_setting);
        double obj_hull_health = -1;
        if (object.exists(cur_setting)) {
          obj_hull_health = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_hull_health;
        }

        cur_setting = "fire";
        object_settings.insert(cur_setting);
        int obj_fire = 0;
        if (object.exists(cur_setting)) {
          obj_fire = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeInt) = obj_fire;
        }

        cur_setting = "cooldown";
        object_settings.insert(cur_setting);
        double obj_cooldown = -1;
        if (object.exists(cur_setting)) {
          obj_cooldown = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_cooldown;
        }

        cur_setting = "fire_immunity";
        object_settings.insert(cur_setting);
        double obj_fire_immunity = -1;
        if (object.exists(cur_setting)) {
          obj_fire_immunity = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_fire_immunity;
        }

        cur_setting = "cmp_t";
        object_settings.insert(cur_setting);
        std::string obj_cmp_t = "armor";
        if (object.exists(cur_setting)) {
          obj_cmp_t = static_cast<const char*>(object[cur_setting]);
          obj_new.add(cur_setting, Setting::TypeString) = obj_cmp_t;
        }

        cur_setting = "part_t_mtp";
        object_settings.insert(cur_setting);
        double obj_part_t_mtp = -1;
        if (object.exists(cur_setting)) {
          obj_part_t_mtp = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeFloat) = obj_part_t_mtp;
        }

        cur_setting = "extinguisher";
        object_settings.insert(cur_setting);
        bool obj_extinguisher = true;
        if (object.exists(cur_setting)) {
          obj_extinguisher = object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeBoolean) = obj_extinguisher;
        }

        cur_setting = "chemspray";
        object_settings.insert(cur_setting);
        if (object.exists(cur_setting)) {
          if (object.exists("extinguisher")) {
            std::cerr << "Use either 'extinguisher' or 'chemspray' as option."
                      << std::endl;
            return 1;
          }
          obj_extinguisher = !object[cur_setting];
          obj_new.add(cur_setting, Setting::TypeBoolean) = !obj_extinguisher;
        }

        cur_setting = "repair_mode";
        object_settings.insert(cur_setting);
        std::string obj_repair_mode = RepairModeString[0];
        if (object.exists(cur_setting)) {
          obj_repair_mode = static_cast<const char*>(object[cur_setting]);
          obj_new.add(cur_setting, Setting::TypeString) = obj_repair_mode;
        }

        cur_setting = "ext_mode";
        object_settings.insert(cur_setting);
        std::string obj_ext_mode = ExtinguishModeString[0];
        if (object.exists(cur_setting)) {
          obj_ext_mode = static_cast<const char*>(object[cur_setting]);
          obj_new.add(cur_setting, Setting::TypeString) = obj_ext_mode;
        }

        if (find_unknown_setting(object_settings, object))
          return 1;

        if (factory_objects.find(obj_name) != factory_objects.end()) {
          std::cout << "Choose different name for different objects: "
                    << obj_name << std::endl;
          return 1;
        }

        Object* obj;
        bool processed = false;
        if (obj_type == "GoioObj") {
          CmpType cmp_type;
          if (!get_cmp_type(obj_cmp_t, cmp_type)) {
            std::cerr << "Invalid component type: " << obj_cmp_t << std::endl;
            return 1;
          }
          obj = ObjectFactory::create(obj_type, obj_name,
                                      cmp_type,
                                      obj_part_t_mtp,
                                      Health(obj_health),
                                      Health(obj_hull_health));
          processed = true;
        } else if (obj_type.substr(0, 9) == "Engineer_") {
          RepairMode mode;
          if (!get_repair_mode(obj_repair_mode, mode)) {
            std::cerr << "Invalid repair mode: "
                      << obj_repair_mode << std::endl;
            return 1;
          }
          ExtinguishMode extmode;
          if (!get_extinguish_mode(obj_ext_mode, extmode)) {
            std::cerr << "Invalid extinguish mode: "
                      << obj_ext_mode << std::endl;
            return 1;
          }
          obj = ObjectFactory::create(obj_type, obj_name,
                                      obj_extinguisher,
                                      mode,
                                      extmode);
          processed = true;
        } else {
          obj = ObjectFactory::create(obj_type, obj_name);
        }
        if (obj == nullptr) {
          std::cerr << "Unknown type: " << obj_type << std::endl;
          return 1;
        }

        factory_objects[obj_name] = obj;
        if (!processed) {
          if (auto goioobj = dynamic_cast<GoioObj*>(obj)) {
            if (obj_health >= 0)
              goioobj->set_health(Health(obj_health));
            if (obj_hull_health >= 0)
              goioobj->set_health(Health(obj_hull_health));
            if (obj_fire > 0)
              goioobj->set_fire(obj_fire);
            if (obj_cooldown >= 0)
              goioobj->add_health(0_hp, Second(obj_cooldown));
            if (obj_fire_immunity >= 0)
              goioobj->add_fire(0, Second(obj_fire_immunity));

            if (obj_ammo != "") {
              if (auto gun = dynamic_cast<Gun*>(obj)) {
                auto it = factory_objects.find(obj_ammo);
                Object* ammo_obj;
                if (it == factory_objects.end()) {
                  ammo_obj = ObjectFactory::create(obj_ammo);
                  factory_objects[obj_ammo] = ammo_obj;
                  if (!dynamic_cast<Ammunition*>(ammo_obj)) {
                    std::cerr << "dynamic_cast to Ammunition failed: "
                              << obj_ammo << std::endl;
                    return 1;
                  }
                } else {
                  ammo_obj = it->second;
                }
                gun->apply_ammunition(static_cast<Ammunition*>(ammo_obj));
              } else {
                std::cerr << "'" << obj_name
                          << "' not a Gun, ammunitions can only be used on guns."
                          << std::endl;
                return 1;
              }
            }
          } else {
            std::cerr << "dynamic_cast to GoioObj failed: "
                      << obj_type << std::endl;
            return 1;
          }
        }
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
        double act_start = -1;
        if (actor.exists(cur_setting)) {
          act_start = actor[cur_setting];
          act_new.add(cur_setting, Setting::TypeFloat) = act_start;
        }

        cur_setting = "end";
        actor_settings.insert(cur_setting);
        double act_end = -1;
        if (actor.exists(cur_setting)) {
          act_end = actor[cur_setting];
          act_new.add(cur_setting, Setting::TypeFloat) = act_end;
        }

        if (find_unknown_setting(actor_settings, actor))
          return 1;

        auto actor_it = factory_objects.find(act_name);
        if (actor_it == factory_objects.end()) {
          std::cerr << "Unknown actor: " << act_name << std::endl;
          return 1;
        }
        auto recipient_it = factory_objects.find(act_recipient);
        if (recipient_it == factory_objects.end()) {
          std::cerr << "Unknown recipient: " << act_recipient << std::endl;
          return 1;
        }
        auto actor_obj = dynamic_cast<GoioActor*>(actor_it->second);
        if (!actor_obj) {
          std::cerr << "dynamic_cast for actor to GoioActor failed: "
                    << act_name << std::endl;
          return 1;
        }
        auto recipient_obj = dynamic_cast<GoioObj*>(recipient_it->second);
        if (!recipient_obj) {
          std::cerr << "dynamic_cast for recipient to GoioObj failed: "
                    << act_recipient << std::endl;
          return 1;
        }

        auto timeobj = std::get<1>(this->simulations.back());

        double reg_start = 0;
        if (act_start > 0)
          reg_start = act_start;

        if (act_action == "shoot") {
          if (auto shoot_actor = dynamic_cast<ShootActor*>(actor_obj)) {
            timeobj->register_shoot_event(shoot_actor,
                                          recipient_obj,
                                          Second(reg_start));
          } else {
            std::cerr << "dynamic_cast for actor to ShootActor failed: "
                      << act_name << std::endl;
            return 1;
          }
        } else if (act_action == "repair") {
          if (auto repair_actor = dynamic_cast<RepairActor*>(actor_obj)) {
            timeobj->register_repair_event(repair_actor,
                                           recipient_obj,
                                           Second(reg_start));
          } else {
            std::cerr << "dynamic_cast for actor to RepairActor failed: "
                      << act_name << std::endl;
            return 1;
          }
        } else {
          std::cerr << "Unknown action: " << act_action << std::endl;
          return 1;
        }

        if (act_end > 0)
          timeobj->unregister_actor(actor_obj, Second(act_end));
      }

      cur_setting = "options";
      simulation_settings.insert(cur_setting);
      if (simulation.exists(cur_setting)) {
        sim_new.add(cur_setting, Setting::TypeGroup);
        const Setting& options = simulation[cur_setting];
        Setting& opt_new = sim_new[cur_setting];

        std::set<std::string> option_settings;
        auto local_options = new Options();

        cur_setting = "max_events";
        option_settings.insert(cur_setting);
        if (options.exists(cur_setting)) {
          int opt_max_events = options[cur_setting];
          opt_new.add(cur_setting, Setting::TypeInt) = opt_max_events;
          local_options->max_events = opt_max_events;
        }

        cur_setting = "max_time";
        option_settings.insert(cur_setting);
        if (options.exists(cur_setting)) {
          double opt_max_time = options[cur_setting];
          opt_new.add(cur_setting, Setting::TypeFloat) = opt_max_time;
          local_options->max_time = opt_max_time;
        }

        std::get<2>(this->simulations.back()) = local_options;

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
        this->options.max_events = opt_max_events;
      }

      cur_setting = "max_time";
      option_settings.insert(cur_setting);
      if (options.exists(cur_setting)) {
        double opt_max_time = options[cur_setting];
        opt_new.add(cur_setting, Setting::TypeFloat) = opt_max_time;
        this->options.max_time = opt_max_time;
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

bool Config::simulate(unsigned int simulation) {
  if (simulation >= get_simulation_count())
    return false;

  auto sim = simulations[simulation];

  std::cout << std::get<0>(sim) << std::endl;
  std::cout << "\033[1m";
  std::cout << "    time          actor  clip health       target      type  health(R) fire type  health(R)" << std::endl;
  std::cout << "===========================================================================================" << std::endl;
  std::cout << "\033[0m";

  Options* opt;
  if (std::get<2>(sim))
    opt = std::get<2>(sim);
  else
    opt = &options;

  int64_t event_count = 0;
  auto timeobj = std::get<1>(sim);
  while (timeobj->next_event()) {
    if (opt->max_events != -1 && ++event_count > opt->max_events) {
      std::cout << "Events limit reached: " << opt->max_events << std::endl;
      return false;
    }
    if (Second(opt->max_time) != -1_s &&
                                timeobj->get_time() > Second(opt->max_time)) {
      std::cout << "Time limit reached: " << opt->max_time << std::endl;
      return false;
    }
  }

  return true;
}

bool Config::simulate_all() {
  auto ret = true;
  for (unsigned int i = 0; i < get_simulation_count(); ++i)
    if (!simulate(i))
      ret = false;
  return ret;
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
