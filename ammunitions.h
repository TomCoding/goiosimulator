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

#ifndef AMMUNITIONS_H_
#define AMMUNITIONS_H_

namespace goio {

class Ammunition {
 protected:
    const double clipsize;
    const double damage;
    const double rof;
    const double aoe_radius;
    const double ign_chance;
    const bool proportional_self_damage;

    Ammunition(double clipsize, double damage, double rof, double aoe_radius,
               double ign_chance, bool proportional_self_damage) :
                      clipsize(clipsize), damage(damage), rof(rof),
                      aoe_radius(aoe_radius), ign_chance(ign_chance),
                      proportional_self_damage(proportional_self_damage) {}

 public:
    virtual ~Ammunition() {}
    inline double get_clipsize() const { return clipsize; }
    inline double get_damage() const { return damage; }
    inline double get_rof() const { return rof; }
    inline double get_aoe_radius() const { return aoe_radius; }
    inline double get_ign_chance() const { return ign_chance; }
    inline bool   get_proportional_self_damage() const {
      return proportional_self_damage;
    }
};

class Lochnagar : public Ammunition {
 public:
    Lochnagar() : Ammunition(0.4,           // magazine
                             2.25,          // damage
                             1,             // rof
                             0.5,           // aoe radius
                             0,             // ign_chance
                             true) {}       // proportional self damage
};

class Incendiary : public Ammunition {
 public:
    Incendiary() : Ammunition(0.75,         // magazine
                             1,             // damage
                             0.7,           // rof
                             1,             // aoe radius
                             0.2,           // ign_chance
                             false) {}      // proportional self damage
};

}  // namespace goio

#endif  // AMMUNITIONS_H_
