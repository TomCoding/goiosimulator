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

#ifndef PRIVATE_H_
#define PRIVATE_H_

#include "./repairtools.h"


namespace goio {

/*
 * Access private members. Solution from:
 * http://bloglitb.blogspot.de/2011/12/access-to-private-members-safer.html
 */
template<typename Tag, typename Tag::type M>
struct Rob {
  friend typename Tag::type get(Tag) {
    return M;
  }
};

struct RepairTool_f {
  typedef int RepairTool::*type;
  friend type get(RepairTool_f);
};

template struct Rob<RepairTool_f, &RepairTool::done>;

}  // namespace goio

#endif  // PRIVATE_H_
