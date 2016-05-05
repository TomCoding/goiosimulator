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

#include "./goioobj.h"


namespace goio {

class Ammunition : public Object {
 protected:
    const double clipsize;
    const double damage;
    const double rof;
    const double aoe_radius;
    const double arming_time;
    const double ign_chance;
    const double projectile_speed;
    const double shell_drop;
    const double jitter;
    const double turn_speed;
    const int    fire_stacks;
    const bool   proportional_self_damage;
    const bool   immune;

    Ammunition(double clipsize, double damage, double rof, double aoe_radius,
               double arming_time, double ign_chance, double projectile_speed,
               double shell_drop, double jitter, double turn_speed,
               int fire_stacks, bool proportional_self_damage, bool immune) :
                      clipsize(clipsize), damage(damage), rof(rof),
                      aoe_radius(aoe_radius), arming_time(arming_time),
                      ign_chance(ign_chance), projectile_speed(projectile_speed),
                      shell_drop(shell_drop), jitter(jitter),
                      turn_speed(turn_speed), fire_stacks(fire_stacks),
                      proportional_self_damage(proportional_self_damage),
                      immune(immune) {}

 public:
    virtual ~Ammunition() {}
    inline double get_clipsize() const { return clipsize; }
    inline double get_damage() const { return damage; }
    inline double get_rof() const { return rof; }
    inline double get_aoe_radius() const { return aoe_radius; }
    inline double get_arming_time() const { return arming_time; }
    inline double get_ign_chance() const { return ign_chance; }
    inline double get_projectile_speed() const { return projectile_speed; }
    inline double get_shell_drop() const { return shell_drop; }
    inline double get_jitter() const { return jitter; }
    inline double get_turn_speed() const { return turn_speed; }
    inline int    get_fire_stacks() const { return fire_stacks; }
    inline bool   get_proportional_self_damage() const {
      return proportional_self_damage;
    }
    inline bool   get_immune() const { return immune; }
};

class Lochnagar : public Ammunition {
 public:
    Lochnagar() : Ammunition(
            0.4,           // magazine
            2.25,          // damage
            1,             // rof
            0.5,           // aoe radius
            0.4,           // arming time
            0,             // ign_chance
            1,             // projectile speed
            1,             // shell drop
            0.4,           // jitter
            0.2,           // rotation speed
            0,             // fire stacks
            true,          // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Greased : public Ammunition {
 public:
    Greased() : Ammunition(
            1.2,           // magazine
            0.8,           // damage
            1.6,           // rof
            1,             // aoe radius
            1,             // arming time
            0,             // ign_chance
            0.8,           // projectile speed
            1,             // shell drop
            1,             // jitter
            0.85,          // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Charged : public Ammunition {
 public:
    Charged() : Ammunition(
            0.8,           // magazine
            1.3,           // damage
            0.75,          // rof
            1,             // aoe radius
            1,             // arming time
            0,             // ign_chance
            1,             // projectile speed
            1,             // shell drop
            1,             // jitter
            1,             // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Lesmok : public Ammunition {
 public:
    Lesmok() : Ammunition(
            0.7,           // magazine
            1,             // damage
            1,             // rof
            1,             // aoe radius
            1,             // arming time
            0,             // ign_chance
            1.7,           // projectile speed
            0.7,           // shell drop
            1,             // jitter
            0.7,           // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Burst : public Ammunition {
 public:
    Burst() : Ammunition(
            1.2,           // magazine
            1,             // damage
            0.85,          // rof
            1.5,           // aoe radius
            1,             // arming time
            0,             // ign_chance
            1,             // projectile speed
            1,             // shell drop
            1,             // jitter
            1,             // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Heavy : public Ammunition {
 public:
    Heavy() : Ammunition(
            0.75,          // magazine
            1,             // damage
            1,             // rof
            1,             // aoe radius
            1,             // arming time
            0,             // ign_chance
            1,             // projectile speed
            1,             // shell drop
            0.3,           // jitter
            1,             // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Incendiary : public Ammunition {
 public:
    Incendiary() : Ammunition(
            0.75,          // magazine
            1,             // damage
            0.7,           // rof
            1,             // aoe radius
            1,             // arming time
            0.2,           // ign_chance
            0.7,           // projectile speed
            1,             // shell drop
            1,             // jitter
            1,             // rotation speed
            0,             // fire stacks
            false,         // proportional self damage
            false          // temporary fire immunity
    ) {}
};

class Heatsink : public Ammunition {
 public:
    Heatsink() : Ammunition(
            1.3,           // magazine
            0.83,          // damage
            1,             // rof
            1,             // aoe radius
            1,             // arming time
            0,             // ign_chance
            0.8,           // projectile speed
            1,             // shell drop
            1,             // jitter
            1.5,           // rotation speed
            -3,            // fire stacks
            false,         // proportional self damage
            true           // temporary fire immunity
    ) {}
};

}  // namespace goio

#endif  // AMMUNITIONS_H_
