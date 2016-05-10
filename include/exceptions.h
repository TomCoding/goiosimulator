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

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <sstream>

#include "./constants.h"


namespace goio {

class NonPositiveTime : public std::exception {
 private:
    Time time;

 public:
    explicit NonPositiveTime(Time time) : time(time) {}

    virtual const char* what() const throw() {
      std::stringstream ss;
      ss << "Non positive time: " << time;
      return ss.str().c_str();
    }
};

class NonPositiveHealth : public std::exception {
 private:
    Health health;

 public:
    explicit NonPositiveHealth(Health health) : health(health) {}

    virtual const char* what() const throw() {
      std::stringstream ss;
      ss << "Non positive time: " << health;
      return ss.str().c_str();
    }
};

}  // namespace goio

#endif // EXCEPTIONS_H_
