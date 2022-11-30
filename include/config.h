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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#include <libconfig.h++>
#include <set>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <string>

#include "./goioobj.h"
#include "./timeobj.h"


namespace goio {

class Config {
 private:
    static constexpr double MIN_COMP_CONFIG_VERSION = 1.0;

    struct Options {
      int64_t max_events;
      double max_time;

      Options() : max_events(-1), max_time(-1) {}
    };

    std::string filename;
    libconfig::Config* cfg_new;
    Options options;

    std::vector<
        std::tuple<std::string,
                   TimeObj*,
                   Options*,
                   std::unordered_map<std::string, Object*>>> simulations;

    bool find_unknown_setting(const std::set<std::string>& settings,
                              const libconfig::Setting& settingsobj);

    Config(const Config& obj) = delete;
    Config& operator=(const Config& obj) = delete;

 public:
    explicit Config(const std::string& filename = "goiovalues.cfg") :
                    filename(filename), cfg_new(new libconfig::Config()),
                    options(), simulations() {}
    ~Config();

    inline const std::string& get_filename() const { return filename; }
    inline unsigned int get_simulation_count() const { return simulations.size(); }

    inline void set_filename(const std::string& filename) {
      this->filename = filename;
    }

    int load_config();
    bool simulate(unsigned int simulation);
    bool simulate_all();

    int write();
};

}  // namespace goio

#endif  // CONFIG_H_
