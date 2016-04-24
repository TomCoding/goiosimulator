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

#ifndef GUNS_H_
#define GUNS_H_

#include <string>

#include "./goioobj.h"
#include "./goioactor.h"
#include "./dmg_types.h"
#include "./ammunitions.h"


namespace goio {

class GunInfo {
 private:
    const int      clipsize;
    const P_Time   rof;
    const Time     reload_;
    const Health   direct_dmg;
    const DmgType  direct_dmg_type;
    const Health   aoe_dmg;
    const DmgType  aoe_dmg_type;
    const Distance aoe_radius;
    const Time     arming_time;
    const double   direct_ign_chance;
    const int      direct_ign_stacks;
    const double   aoe_ign_chance;
    const int      aoe_ign_stacks;
    const Distance range;
    const Speed    projectile_speed;
    const Acceleration shell_drop;
    const Angle    jitter;
    const Angle    turn_left;
    const Angle    turn_right;
    const Angle    turn_up;
    const Angle    turn_down;
    const Angular_Speed turn_horizontal;
    const Angular_Speed turn_vertical;

 protected:
    GunInfo(int clipsize,
            P_Time rof,
            Time reload,
            Health direct_dmg,
            DmgType direct_dmg_type,
            Health aoe_dmg,
            DmgType aoe_dmg_type,
            Distance aoe_radius,
            Time arming_time,
            double direct_ign_chance,
            int direct_ign_stacks,
            double aoe_ign_chance,
            int aoe_ign_stacks,
            Distance range,
            Speed projectile_speed,
            Acceleration shell_drop,
            Angle jitter,
            Angle turn_left,
            Angle turn_right,
            Angle turn_up,
            Angle turn_down,
            Angular_Speed turn_horizontal,
            Angular_Speed turn_vertical) :
                                   clipsize(clipsize),
                                   rof(rof),
                                   reload_(reload),
                                   direct_dmg(direct_dmg),
                                   direct_dmg_type(direct_dmg_type),
                                   aoe_dmg(aoe_dmg),
                                   aoe_dmg_type(aoe_dmg_type),
                                   aoe_radius(aoe_radius),
                                   arming_time(arming_time),
                                   direct_ign_chance(direct_ign_chance),
                                   direct_ign_stacks(direct_ign_stacks),
                                   aoe_ign_chance(aoe_ign_chance),
                                   aoe_ign_stacks(aoe_ign_stacks),
                                   range(range),
                                   projectile_speed(projectile_speed),
                                   shell_drop(shell_drop),
                                   jitter(jitter),
                                   turn_left(turn_left),
                                   turn_right(turn_right),
                                   turn_up(turn_up),
                                   turn_down(turn_down),
                                   turn_horizontal(turn_horizontal),
                                   turn_vertical(turn_vertical) {}
    virtual ~GunInfo() {}

 public:
    inline int      get_orig_clipsize() const { return clipsize; }
    inline P_Time   get_orig_rof() const { return rof; }
    inline Time     get_orig_reload() const { return reload_; }
    inline Health   get_orig_direct_dmg() const { return direct_dmg; }
    inline DmgType  get_orig_direct_dmg_type() const { return direct_dmg_type; }
    inline Health   get_orig_aoe_dmg() const { return aoe_dmg; }
    inline DmgType  get_orig_aoe_dmg_type() const { return aoe_dmg_type; }
    inline Time     get_orig_arming_time() const { return arming_time; }
    inline Distance get_orig_aoe_radius() const { return aoe_radius; }
    inline double   get_orig_direct_ign_chance() const { return direct_ign_chance; }
    inline int      get_orig_direct_ign_stacks() const { return direct_ign_stacks; }
    inline double   get_orig_aoe_ign_chance() const { return aoe_ign_chance; }
    inline int      get_orig_aoe_ign_stacks() const { return aoe_ign_stacks; }
    inline Distance get_orig_range() const { return range; }
    inline Speed    get_orig_projectile_speed() const { return projectile_speed; }
    inline Acceleration get_orig_shell_drop() const { return shell_drop; }
    inline Angle    get_orig_jitter() const { return jitter; }
    inline Angle    get_orig_turn_left() const { return turn_left; }
    inline Angle    get_orig_turn_right() const { return turn_right; }
    inline Angle    get_orig_turn_up() const { return turn_up; }
    inline Angle    get_orig_turn_down() const { return turn_down; }
    inline Angular_Speed get_orig_turn_horizontal() const { return turn_horizontal; }
    inline Angular_Speed get_orig_turn_vertical() const { return turn_vertical; }
};

class Gun : public GunInfo, public ShootActor {
 private:
    int      cur_clipsize;
    P_Time   cur_rof;
    Time     cur_reload;
    Health   cur_direct_dmg;
    DmgType  cur_direct_dmg_type;
    Health   cur_aoe_dmg;
    DmgType  cur_aoe_dmg_type;
    Distance cur_aoe_radius;
    Time     cur_arming_time;
    double   cur_direct_ign_chance;
    double   cur_aoe_ign_chance;
    Distance cur_range;
    Speed    cur_projectile_speed;
    Acceleration cur_shell_drop;
    Angle    cur_jitter;
    Angle    cur_turn_left;
    Angle    cur_turn_right;
    Angle    cur_turn_up;
    Angle    cur_turn_down;
    Angular_Speed cur_turn_horizontal;
    Angular_Speed cur_turn_vertical;

    const Ammunition* cur_ammo;

    bool during_reload;

    Gun(const Gun& obj);
    Gun& operator=(const Gun& obj);

    void set_clipsize(int clipsize);
    void set_rof(P_Time rof);
    void set_reload(Time reload);
    void set_direct_dmg(Health direct_dmg);
    void set_direct_dmg_type(DmgType direct_dmg_type);
    void set_aoe_dmg(Health aoe_dmg);
    void set_aoe_dmg_type(DmgType aoe_dmg_type);
    void set_aoe_radius(Distance aoe_radius);
    void set_arming_time(Time arming_time);
    void set_direct_ign_chance(double ign_chance);
    void set_aoe_ign_chance(double ign_chance);
    void set_range(Distance range);
    void set_projectile_speed(Speed projectile_speed);
    void set_shell_drop(Acceleration shell_drop);
    void set_jitter(Angle jitter);
    void set_turn_left(Angle turn_left);
    void set_turn_right(Angle turn_right);
    void set_turn_up(Angle turn_up);
    void set_turn_down(Angle turn_down);
    void set_turn_horizontal(Angular_Speed turn_horizontal);
    void set_turn_vertical(Angular_Speed turn_vertical);

    inline int dec_clipsize() {
      if (--cur_clipsize < 0)
        cur_clipsize = 0;
      return cur_clipsize;
    }

 protected:
    Gun(const std::string& name, Health max_health,
        int clipsize, P_Time rof, Time reload, Health direct_dmg,
        DmgType direct_dmg_type, Health aoe_dmg, DmgType aoe_dmg_type,
        Distance aoe_radius, Time arming_time,
        double direct_ign_chance, int direct_ign_stacks,
        double aoe_ign_chance, int aoe_ign_stacks,
        Distance range, Speed projectile_speed,
        Acceleration shell_drop, Angle jitter,
        Angle turn_left, Angle turn_right, Angle turn_up, Angle turn_down,
        Angular_Speed turn_horizontal, Angular_Speed turn_vertical) :
                GunInfo(clipsize, rof, reload, direct_dmg, direct_dmg_type,
                        aoe_dmg, aoe_dmg_type, aoe_radius, arming_time,
                        direct_ign_chance, direct_ign_stacks,
                        aoe_ign_chance, aoe_ign_stacks,
                        range, projectile_speed, shell_drop, jitter,
                        turn_left, turn_right, turn_up, turn_down,
                        turn_horizontal, turn_vertical),
                ShootActor(name, CmpType::COMPONENTS, 2.33, max_health),
                cur_clipsize(clipsize),
                cur_rof(rof),
                cur_reload(reload),
                cur_direct_dmg(direct_dmg),
                cur_direct_dmg_type(direct_dmg_type),
                cur_aoe_dmg(aoe_dmg),
                cur_aoe_dmg_type(aoe_dmg_type),
                cur_aoe_radius(aoe_radius),
                cur_arming_time(arming_time),
                cur_direct_ign_chance(direct_ign_chance),
                cur_aoe_ign_chance(aoe_ign_chance),
                cur_range(range),
                cur_projectile_speed(projectile_speed),
                cur_shell_drop(shell_drop),
                cur_jitter(jitter),
                cur_turn_left(turn_left),
                cur_turn_right(turn_right),
                cur_turn_up(turn_up),
                cur_turn_down(turn_down),
                cur_turn_horizontal(turn_horizontal),
                cur_turn_vertical(turn_vertical),
                cur_ammo(nullptr),
                during_reload(false) {}

 public:
    virtual ~Gun() {}

    int             get_max_clipsize() const;
    inline int      get_clipsize() const { return cur_clipsize; }
    inline P_Time   get_rof() const { return cur_rof; }
    inline Time     get_reload() const { return cur_reload; }
    inline Health   get_direct_dmg() const { return cur_direct_dmg; }
    inline DmgType  get_direct_dmg_type() const { return cur_direct_dmg_type; }
    inline Health   get_aoe_dmg() const { return cur_aoe_dmg; }
    inline DmgType  get_aoe_dmg_type() const { return cur_aoe_dmg_type; }
    inline Distance get_aoe_radius() const { return cur_aoe_radius; }
    inline Time     get_arming_time() const { return cur_arming_time; }
    inline double   get_direct_ign_chance() const { return cur_direct_ign_chance; }
    inline double   get_aoe_ign_chance() const { return cur_aoe_ign_chance; }
    inline Distance get_range() const { return cur_range; }
    inline Speed    get_projectile_speed() const { return cur_projectile_speed; }
    inline Acceleration get_shell_drop() const { return cur_shell_drop; }
    inline Angle    get_jitter() const { return cur_jitter; }
    inline Angle    get_turn_left() const { return cur_turn_left; }
    inline Angle    get_turn_right() const { return cur_turn_right; }
    inline Angle    get_turn_up() const { return cur_turn_up; }
    inline Angle    get_turn_down() const { return cur_turn_down; }
    inline Angular_Speed get_turn_horizontal() const { return cur_turn_horizontal; }
    inline Angular_Speed get_turn_vertical() const { return cur_turn_vertical; }
    inline const Ammunition* get_ammo() const { return cur_ammo; }

    bool apply_ammunition(const Ammunition* ammo);

    void reload(bool with_ammo = true);

    P_Time get_rof_changed() const;
    Time get_time_per_shot() const;
    Time get_reload_changed() const;
    Angular_Speed get_turn_horizontal_changed() const;
    Angular_Speed get_turn_vertical_changed() const;

    DmgState::State shoot(GoioObj* obj, Time,
                          bool armed, Distance aoe_range) override;

    TimeFunc get_time_func(const GoioObj*, Time, bool&) override;
};

class LightGun : public Gun {
 protected:
    LightGun(const std::string& name, int clipsize, P_Time rof, Time reload,
             Health direct_dmg, DmgType direct_dmg_type, Health aoe_dmg,
             DmgType aoe_dmg_type, Distance aoe_radius, Time arming_time,
             double direct_ign_chance, int direct_ign_stacks,
             double aoe_ign_chance, int aoe_ign_stacks,
             Distance range, Speed projectile_speed,
             Acceleration shell_drop = 0_m_s2, Angle jitter = 0_deg,
             Angle turn_left = 0_deg, Angle turn_right = 0_deg,
             Angle turn_up = 0_deg, Angle turn_down = 0_deg,
             Angular_Speed turn_horizontal = 0_deg/1_s,
             Angular_Speed turn_vertical = 0_deg/1_s) :
                Gun(name, 250_hp, clipsize, rof, reload,
                    direct_dmg, direct_dmg_type,
                    aoe_dmg, aoe_dmg_type, aoe_radius, arming_time,
                    direct_ign_chance, direct_ign_stacks,
                    aoe_ign_chance, aoe_ign_stacks,
                    range, projectile_speed, shell_drop, jitter,
                    turn_left, turn_right, turn_up, turn_down,
                    turn_horizontal, turn_vertical) {}

 public:
    ~LightGun() {}
};

class Artemis : public LightGun {
 public:
    explicit Artemis(const std::string& name) : LightGun(
                    name,
                    4,                   // magazine size
                    0.63/1_s,            // rof
                    7_s,                 // reload
                    70_hp,               // prim dmg
                    DmgType::EXPLOSIVE,  // prim dmg type
                    120_hp,              // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    2.5_m,               // aoe radius
                    0_s,                 // arming time
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1,                   // sec ign stacks
                    1334_m,              // range
                    575_m/1_s,           // projectile speed
                    0_m_s2,              // shell_drop
                    0_deg                // jitter
    ) {}
};

class Banshee : public LightGun {
 public:
    explicit Banshee(const std::string& name) : LightGun(
                    name,
                    8,                   // magazine size
                    2/1_s,               // rof
                    6_s,                 // reload
                    25_hp,               // prim dmg
                    DmgType::EXPLOSIVE,  // prim dmg type
                    25_hp,               // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    3_m,                 // aoe radius
                    0_s,                 // arming time
                    0.35,                // prim ign chance
                    1,                   // prim ign stacks
                    0.264,               // sec ign chance
                    2,                   // sec ign stacks
                    1334_m,              // range
                    450_m/1_s,           // projectile speed
                    0_m_s2,              // shell_drop
                    2_deg                // jitter
    ) {}
};

class LightCaro : public LightGun {
 public:
    explicit LightCaro(const std::string& name) : LightGun(
                    name,
                    5,                   // magazine size
                    1/1_s,               // rof
                    6_s,                 // reload
                    96_hp,               // prim dmg
                    DmgType::FLECHETTE,  // prim dmg type
                    144_hp,              // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    0_m,                 // aoe radius
                    0_s,                 // arming time
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1,                   // sec ign stacks
                    325_m,               // range
                    600_m/1_s,           // projectile speed
                    6_m_s2,              // shell_drop
                    5_deg                // jitter
    ) {}
};

class Flare : public LightGun {
 public:
    explicit Flare(const std::string& name) : LightGun(
                    name,
                    2,                   // magazine size
                    0.5/1_s,             // rof
                    20_s,                // reload
                    10_hp,               // prim dmg
                    DmgType::FIRE,       // prim dmg type
                    5_hp,                // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    3_m,                 // aoe radius
                    0.6666_s,            // arming time
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1,                   // sec ign stacks
                    750_m,               // range
                    300_m/1_s,           // projectile speed
                    3_m_s2,              // shell_drop
                    5_deg                // jitter
    ) {}
};

class Flamethrower : public LightGun {
 public:
    explicit Flamethrower(const std::string& name) : LightGun(
                    name,
                    250,                 // magazine size
                    16.67/1_s,           // rof
                    6_s,                 // reload
                    1.2_hp,              // prim dmg
                    DmgType::FIRE,       // prim dmg type
                    0_hp,                // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    4_m,                 // aoe radius
                    0_s,                 // arming time
                    0.22,                // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1,                   // sec ign stacks
                    200_m,               // range
                    180_m/1_s,           // projectile speed
                    0_m_s2,              // shell_drop
                    6_deg                // jitter
    ) {}
};

class Gatling : public LightGun {
 public:
    explicit Gatling(const std::string& name) : LightGun(
                    name,
                    82,                  // magazine size
                    8.33/1_s,            // rof
                    5_s,                 // reload
                    7.5_hp,              // prim dmg
                    DmgType::PIERCING,   // prim dmg type
                    10_hp,               // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    0_m,                 // aoe radius
                    0_s,                 // arming time
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1,                   // sec ign stacks
                    450_m,               // range
                    500_m/1_s,           // projectile speed
                    0_m_s2,              // shell_drop
                    2.25_deg             // jitter
    ) {}
};

}  //  namespace goio

#endif  // GUNS_H_
