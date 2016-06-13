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

#ifndef TOOL_H_
#define TOOL_H_


namespace goio {

class ToolDispatcher;
class Ship;
class Engine;
class Balloon;
class Gun;

class Tool {
 protected:
    Tool() {}
    ~Tool() {}

 public:
    virtual void accept(ToolDispatcher& dispatcher, Ship* obj, bool activate) const = 0;
    virtual void accept(ToolDispatcher& dispatcher, Engine* obj, bool activate) const = 0;
    virtual void accept(ToolDispatcher& dispatcher, Balloon* obj, bool activate) const = 0;
    virtual void accept(ToolDispatcher& dispatcher, Gun* obj, bool activate) const = 0;
};

}  // namespace goio


#endif  // TOOL_H_
