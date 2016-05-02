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

#ifndef ENGINES_H_
#define ENGINES_H_

#include <string>

#include "./goioobj.h"
#include "./dmg_types.h"


namespace goio {

class EngineInfo {
 private:
    const Thrust thrust;

 protected:
    EngineInfo(Thrust thrust) : thrust(thrust) {}
    virtual ~EngineInfo() {}

 public:
    inline Thrust get_orig_thrust() const { return thrust; }
};

class Engine : public EngineInfo, public GoioObj {
 private:
    Thrust cur_thrust;

    Engine(const Engine& obj);
    Engine& operator=(const Engine& obj);

    void set_thrust(Thrust thrust);

 protected:
    Engine(const std::string& name, Health max_health, Thrust thrust) :
              EngineInfo(thrust),
              GoioObj(name, CmpType::COMPONENTS, 1.333333, max_health),
              cur_thrust(thrust) {}
    virtual ~Engine() {}

 public:
    inline Thrust get_thrust() const { return cur_thrust; }

    Thrust get_thrust_changed() const;

    void reset(bool) override;
};

class LightEngine : public Engine {
 public:
    LightEngine(const std::string& name, Thrust thrust) :
              Engine(name, 300_hp, thrust) {}
};

class HeavyEngine : public Engine {
 public:
    HeavyEngine(const std::string& name, Thrust thrust) :
              Engine(name, 525_hp, thrust) {}
};

}  // namespace goio


#endif  // ENGINES_H_
