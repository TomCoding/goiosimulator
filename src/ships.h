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

#ifndef SHIPS_H_
#define SHIPS_H_

#include <string>

#include "./goioobj.h"


namespace goio {

class Ship : public GoioObj {
 protected:
    Ship(const std::string& name, Health max_health, Health hull_max_health) :
              GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health) {}

 public:
    virtual ~Ship() {}
};

class Pyramidion : public Ship {
 public:
    explicit Pyramidion(const std::string& name) : Ship(name, 650_hp, 700_hp) {}
};

class Goldfish : public Ship {
 public:
    explicit Goldfish(const std::string& name) : Ship(name, 400_hp, 1100_hp) {}
};

class Junker : public Ship {
 public:
    explicit Junker(const std::string& name) : Ship(name, 700_hp, 500_hp) {}
};

class Galleon : public Ship {
 public:
    explicit Galleon(const std::string& name) : Ship(name, 800_hp, 1400_hp) {}
};

class Squid : public Ship {
 public:
    explicit Squid(const std::string& name) : Ship(name, 230_hp, 950_hp) {}
};

class Spire : public Ship {
 public:
    explicit Spire(const std::string& name) : Ship(name, 400_hp, 950_hp) {}
};

class Mobula : public Ship {
 public:
    explicit Mobula(const std::string& name) : Ship(name, 600_hp, 700_hp) {}
};

}  // namespace goio

#endif  // SHIPS_H_
