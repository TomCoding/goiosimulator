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

#ifndef UNTILS_H_
#define UNTILS_H_

#include <type_traits>


// Solution from http://stackoverflow.com/a/31836401/770468
template <typename C, C beginVal, C endVal>
class Iterator {
 private:
    typedef typename std::underlying_type<C>::type val_t;
    int val;

 public:
    Iterator(const C& f) : val(static_cast<val_t>(f)) {}
    Iterator() : val(static_cast<val_t>(beginVal)) {
      static_assert(beginVal <= endVal, "requirement 'beginVal <= endVal' not met");
    }
    Iterator& operator++() {
      ++val;
      return *this;
    }
    C operator*() { return static_cast<C>(val); }
    Iterator begin() { return *this; }  // default ctor is good
    Iterator end() {
      static const Iterator endIter = ++Iterator(endVal);  // cache it
      return endIter;
    }
    bool operator!=(const Iterator& i) { return val != i.val; }
};

#endif  // UNTILS_H_
