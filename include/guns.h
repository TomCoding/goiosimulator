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

enum class GunType {
  PROJECTILE,
  RAYCAST,
  PARTICLE
};

enum class GunCategory {
  MORTARS,
  CARRONADES,
  FLAKS,
  ROCKET_LAUNCHERS,
  UTILITY,
  OTHERS
};

class GunInfo {
 private:
    const GunType  type;
    const GunCategory category;
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
    GunInfo(GunType type,
            GunCategory category,
            int clipsize,
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
                                   type(type),
                                   category(category),
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
    inline GunType  get_type() const { return type; }
    inline GunCategory get_category() const { return category; }
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
    inline Angular_Speed get_orig_turn_horizontal() const {
      return turn_horizontal;
    }
    inline Angular_Speed get_orig_turn_vertical() const {
      return turn_vertical;
    }
};

class Gun : public GunInfo, public ShootActor {
 private:
    double  cur_clipsize;
    double  cur_clipsize_mod;
    double  cur_rof_mod;
    double  cur_reload_mod;
    double  cur_direct_dmg_mod;
    DmgType cur_direct_dmg_type;
    double  cur_aoe_dmg_mod;
    DmgType cur_aoe_dmg_type;
    double  cur_aoe_radius_mod;
    double  cur_arming_time_mod;
    double  cur_direct_ign_chance_mod;
    double  cur_aoe_ign_chance_mod;
    double  cur_range_mod;
    double  cur_projectile_speed_mod;
    double  cur_shell_drop_mod;
    double  cur_jitter_mod;
    double  cur_turn_left_mod;
    double  cur_turn_right_mod;
    double  cur_turn_up_mod;
    double  cur_turn_down_mod;
    double  cur_turn_horizontal_mod;
    double  cur_turn_vertical_mod;

    const Ammunition* cur_ammo;

    bool during_reload;

    Gun(const Gun& obj);
    Gun& operator=(const Gun& obj);

    void add_clipsize_mod(double clipsize);
    void add_rof_mod(double rof);
    void add_reload_mod(double reload);
    void add_direct_dmg_mod(double direct_dmg);
    void add_aoe_dmg_mod(double aoe_dmg);
    void add_aoe_radius_mod(double aoe_radius);
    void add_arming_time_mod(double arming_time);
    void add_direct_ign_chance_mod(double ign_chance);
    void add_aoe_ign_chance_mod(double ign_chance);
    void add_range_mod(double range);
    void add_projectile_speed_mod(double projectile_speed);
    void add_shell_drop_mod(double shell_drop);
    void add_jitter_mod(double jitter);
    void add_turn_left_mod(double turn_left);
    void add_turn_right_mod(double turn_right);
    void add_turn_up_mod(double turn_up);
    void add_turn_down_mod(double turn_down);
    void add_turn_horizontal_mod(double turn_horizontal);
    void add_turn_vertical_mod(double turn_vertical);

    void set_direct_dmg_type(DmgType direct_dmg_type);
    void set_aoe_dmg_type(DmgType aoe_dmg_type);

    inline int dec_clipsize() {
      if (--cur_clipsize < 0)
        cur_clipsize = 0;
      return cur_clipsize;
    }

 protected:
    Gun(const std::string& name, Health max_health,
        GunType type, GunCategory category,
        int clipsize, P_Time rof, Time reload, Health direct_dmg,
        DmgType direct_dmg_type, Health aoe_dmg, DmgType aoe_dmg_type,
        Distance aoe_radius, Time arming_time,
        double direct_ign_chance, int direct_ign_stacks,
        double aoe_ign_chance, int aoe_ign_stacks,
        Distance range, Speed projectile_speed,
        Acceleration shell_drop, Angle jitter,
        Angle turn_left, Angle turn_right, Angle turn_up, Angle turn_down,
        Angular_Speed turn_horizontal, Angular_Speed turn_vertical) :
                GunInfo(type, category,
                        clipsize, rof, reload, direct_dmg, direct_dmg_type,
                        aoe_dmg, aoe_dmg_type, aoe_radius, arming_time,
                        direct_ign_chance, direct_ign_stacks,
                        aoe_ign_chance, aoe_ign_stacks,
                        range, projectile_speed, shell_drop, jitter,
                        turn_left, turn_right, turn_up, turn_down,
                        turn_horizontal, turn_vertical),
                ShootActor(name, CmpType::COMPONENTS, 2.333333, max_health),
                cur_clipsize(clipsize),
                cur_clipsize_mod(0),
                cur_rof_mod(0),
                cur_reload_mod(0),
                cur_direct_dmg_mod(0),
                cur_direct_dmg_type(direct_dmg_type),
                cur_aoe_dmg_mod(0),
                cur_aoe_dmg_type(aoe_dmg_type),
                cur_aoe_radius_mod(0),
                cur_arming_time_mod(0),
                cur_direct_ign_chance_mod(0),
                cur_aoe_ign_chance_mod(0),
                cur_range_mod(0),
                cur_projectile_speed_mod(0),
                cur_shell_drop_mod(0),
                cur_jitter_mod(0),
                cur_turn_left_mod(0),
                cur_turn_right_mod(0),
                cur_turn_up_mod(0),
                cur_turn_down_mod(0),
                cur_turn_horizontal_mod(0),
                cur_turn_vertical_mod(0),
                cur_ammo(nullptr),
                during_reload(false) {}

 public:
    virtual ~Gun() {}

    inline int      get_max_clipsize() const { 
      return get_orig_clipsize() * (1+cur_clipsize_mod);
    }
    inline int      get_clipsize() const { return cur_clipsize; }
    inline P_Time   get_rof() const {
      return get_orig_rof() * (1+cur_rof_mod);
    }
    inline Time     get_reload() const {
      return get_orig_reload() * (1+cur_reload_mod);
    }
    inline Health   get_direct_dmg() const {
      return get_orig_direct_dmg() * (1+cur_direct_dmg_mod);
    }
    inline DmgType  get_direct_dmg_type() const { return cur_direct_dmg_type; }
    inline Health   get_aoe_dmg() const {
      return get_orig_aoe_dmg() * (1+cur_aoe_dmg_mod);
    }
    inline DmgType  get_aoe_dmg_type() const { return cur_aoe_dmg_type; }
    inline Distance get_aoe_radius() const {
      return get_orig_aoe_radius() * (1+cur_aoe_radius_mod);
    }
    inline Time     get_arming_time() const {
      return get_orig_arming_time() * (1+cur_arming_time_mod);
    }
    inline double   get_direct_ign_chance() const {
      return get_orig_direct_ign_chance() + cur_direct_ign_chance_mod;
    }
    inline double   get_aoe_ign_chance() const {
      return get_orig_aoe_ign_chance() + cur_aoe_ign_chance_mod;
    }
    inline Distance get_range() const {
      return get_orig_range() * (1+cur_range_mod);
    }
    inline Speed    get_projectile_speed() const {
      return get_orig_projectile_speed() * (1+cur_projectile_speed_mod);
    }
    inline Acceleration get_shell_drop() const {
      return get_orig_shell_drop() * (1+cur_shell_drop_mod);
    }
    inline Angle    get_jitter() const {
      return get_orig_jitter() * (1+cur_jitter_mod);
    }
    inline Angle    get_turn_left() const {
      return get_orig_turn_left() * (1+cur_turn_left_mod);
    }
    inline Angle    get_turn_right() const {
      return get_orig_turn_right() * (1+cur_turn_right_mod);
    }
    inline Angle    get_turn_up() const {
      return get_orig_turn_up() * (1+cur_turn_up_mod);
    }
    inline Angle    get_turn_down() const {
      return get_orig_turn_down() * (1+cur_turn_down_mod);
    }
    inline Angular_Speed get_turn_horizontal() const {
      return get_orig_turn_horizontal() * (1+cur_turn_horizontal_mod);
    }
    inline Angular_Speed get_turn_vertical() const {
      return get_orig_turn_vertical() * (1+cur_turn_vertical_mod);
    }
    inline const Ammunition* get_ammo() const { return cur_ammo; }

    void reload(const Ammunition* ammo = nullptr);

    P_Time        get_rof_changed() const;
    Time          get_time_per_shot() const;
    Time          get_reload_changed() const;
    Angular_Speed get_turn_horizontal_changed() const;
    Angular_Speed get_turn_vertical_changed() const;

    DmgState shoot(GoioObj* obj, Time,
                          bool armed, Distance aoe_range) override;

    TimeFunc get_time_func(const GoioObj*, Time, bool&) override;
};

class LightGun : public Gun {
 protected:
    LightGun(const std::string& name, GunType type, GunCategory category,
             int clipsize, P_Time rof, Time reload,
             Health direct_dmg, DmgType direct_dmg_type,
             Health aoe_dmg, DmgType aoe_dmg_type,
             Distance aoe_radius, Time arming_time,
             double direct_ign_chance, int direct_ign_stacks,
             double aoe_ign_chance, int aoe_ign_stacks,
             Distance range, Speed projectile_speed,
             Acceleration shell_drop = 0_m_s2, Angle jitter = 0_deg,
             Angle turn_left = 0_deg, Angle turn_right = 0_deg,
             Angle turn_up = 0_deg, Angle turn_down = 0_deg,
             Angular_Speed turn_horizontal = 0_deg/1_s,
             Angular_Speed turn_vertical = 0_deg/1_s) :
                Gun(name, 250_hp, type, category, clipsize, rof, reload,
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

class HeavyGun : public Gun {
 protected:
    HeavyGun(const std::string& name, GunType type, GunCategory category,
             int clipsize, P_Time rof, Time reload,
             Health direct_dmg, DmgType direct_dmg_type,
             Health aoe_dmg, DmgType aoe_dmg_type,
             Distance aoe_radius, Time arming_time,
             double direct_ign_chance, int direct_ign_stacks,
             double aoe_ign_chance, int aoe_ign_stacks,
             Distance range, Speed projectile_speed,
             Acceleration shell_drop = 0_m_s2, Angle jitter = 0_deg,
             Angle turn_left = 0_deg, Angle turn_right = 0_deg,
             Angle turn_up = 0_deg, Angle turn_down = 0_deg,
             Angular_Speed turn_horizontal = 0_deg/1_s,
             Angular_Speed turn_vertical = 0_deg/1_s) :
                Gun(name, 425_hp, type, category, clipsize, rof, reload,
                    direct_dmg, direct_dmg_type,
                    aoe_dmg, aoe_dmg_type, aoe_radius, arming_time,
                    direct_ign_chance, direct_ign_stacks,
                    aoe_ign_chance, aoe_ign_stacks,
                    range, projectile_speed, shell_drop, jitter,
                    turn_left, turn_right, turn_up, turn_down,
                    turn_horizontal, turn_vertical) {}

 public:
    ~HeavyGun() {}
};

class Artemis : public LightGun {
 public:
    explicit Artemis(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::ROCKET_LAUNCHERS,  // gun category
                4,                       // magazine size
                0.63/1_s,                // rof
                7_s,                     // reload
                70_hp,                   // prim dmg
                DmgType::EXPLOSIVE,      // prim dmg type
                120_hp,                  // sec dmg
                DmgType::SHATTER,        // sec dmg type
                2.5_m,                   // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                1334_m,                  // range
                575_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                0_deg,                   // jitter
                65_deg,                  // left turn angle
                65_deg,                  // right turn angle
                10_deg,                  // upwards turn angle
                35_deg,                  // downwards turn angle
                25_deg/1_s,              // horizontal turn speed
                80_deg/1_s               // vertical turn speed
    ) {}
};

class Banshee : public LightGun {
 public:
    explicit Banshee(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::ROCKET_LAUNCHERS,  // gun category
                8,                       // magazine size
                2/1_s,                   // rof
                6_s,                     // reload
                25_hp,                   // prim dmg
                DmgType::EXPLOSIVE,      // prim dmg type
                25_hp,                   // sec dmg
                DmgType::FIRE,           // sec dmg type
                3_m,                     // aoe radius
                0_s,                     // arming time
                0.35,                    // prim ign chance
                1,                       // prim ign stacks
                0.264,                   // sec ign chance
                2,                       // sec ign stacks
                1334_m,                  // range
                450_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                2_deg,                   // jitter
                60_deg,                  // left turn angle
                60_deg,                  // right turn angle
                35_deg,                  // upwards turn angle
                35_deg,                  // downwards turn angle
                130_deg/1_s,             // horizontal turn speed
                140_deg/1_s              // vertical turn speed
    ) {}
};

class LightCaro : public LightGun {
 public:
    explicit LightCaro(const std::string& name) : LightGun(
                name,
                GunType::RAYCAST,        // gun type
                GunCategory::CARRONADES,  // gun category
                5,                       // magazine size
                1/1_s,                   // rof
                6_s,                     // reload
                96_hp,                   // prim dmg
                DmgType::FLECHETTE,      // prim dmg type
                144_hp,                  // sec dmg
                DmgType::SHATTER,        // sec dmg type
                0_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                325_m,                   // range
                600_m/1_s,               // projectile speed
                6_m_s2,                  // shell_drop
                5_deg,                   // jitter
                55_deg,                  // left turn angle
                55_deg,                  // right turn angle
                30_deg,                  // upwards turn angle
                15_deg,                  // downwards turn angle
                100_deg/1_s,             // horizontal turn speed
                80_deg/1_s               // vertical turn speed
    ) {}
};

class Flare : public LightGun {
 public:
    explicit Flare(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::UTILITY,    // gun category
                2,                       // magazine size
                0.5/1_s,                 // rof
                20_s,                    // reload
                10_hp,                   // prim dmg
                DmgType::FIRE,           // prim dmg type
                5_hp,                    // sec dmg
                DmgType::FIRE,           // sec dmg type
                3_m,                     // aoe radius
                0.6666_s,                // arming time
                1,                       // prim ign chance
                15,                      // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                750_m,                   // range
                300_m/1_s,               // projectile speed
                3_m_s2,                  // shell_drop
                5_deg,                   // jitter
                80_deg,                  // left turn angle
                80_deg,                  // right turn angle
                50_deg,                  // upwards turn angle
                50_deg,                  // downwards turn angle
                190_deg/1_s,             // horizontal turn speed
                150_deg/1_s              // vertical turn speed
    ) {}
};

class Flamethrower : public LightGun {
 public:
    explicit Flamethrower(const std::string& name) : LightGun(
                name,
                GunType::PARTICLE,       // gun type
                GunCategory::OTHERS,     // gun category
                250,                     // magazine size
                16.67/1_s,               // rof
                6_s,                     // reload
                1.2_hp,                  // prim dmg
                DmgType::FIRE,           // prim dmg type
                0_hp,                    // sec dmg
                DmgType::FIRE,           // sec dmg type
                4_m,                     // aoe radius
                0_s,                     // arming time
                0.22,                    // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                200_m,                   // range
                180_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                6_deg,                   // jitter
                60_deg,                  // left turn angle
                60_deg,                  // right turn angle
                35_deg,                  // upwards turn angle
                35_deg,                  // downwards turn angle
                40_deg/1_s,              // horizontal turn speed
                50_deg/1_s               // vertical turn speed
    ) {}
};

class LightFlak : public LightGun {
 public:
    explicit LightFlak(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::FLAKS,      // gun category
                6,                       // magazine size
                2.857/1_s,               // rof
                4_s,                     // reload
                50_hp,                   // prim dmg
                DmgType::EXPLOSIVE,      // prim dmg type
                30_hp,                   // sec dmg
                DmgType::EXPLOSIVE,      // sec dmg type
                4_m,                     // aoe radius
                0.4285_s,                // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                999.95_m,                // range
                350_m/1_s,               // projectile speed
                5_m_s2,                  // shell_drop
                1_deg,                   // jitter
                50_deg,                  // left turn angle
                50_deg,                  // right turn angle
                35_deg,                  // upwards turn angle
                35_deg,                  // downwards turn angle
                50_deg/1_s,              // horizontal turn speed
                50_deg/1_s               // vertical turn speed
    ) {}
};

class Hades : public LightGun {
 public:
    explicit Hades(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::OTHERS,     // gun category
                8,                       // magazine size
                1.25/1_s,                // rof
                5.5_s,                   // reload
                30_hp,                   // prim dmg
                DmgType::FIRE,           // prim dmg type
                45_hp,                   // sec dmg
                DmgType::PIERCING,       // sec dmg type
                5_m,                     // aoe radius
                0.6_s,                   // arming time
                0.25,                    // prim ign chance
                1,                       // prim ign stacks
                0.4,                     // sec ign chance
                1,                       // sec ign stacks
                1400_m,                  // range
                250_m/1_s,               // projectile speed
                15_m_s2,                 // shell_drop
                0_deg,                   // jitter
                35_deg,                  // left turn angle
                35_deg,                  // right turn angle
                40_deg,                  // upwards turn angle
                50_deg,                  // downwards turn angle
                50_deg/1_s,              // horizontal turn speed
                70_deg/1_s               // vertical turn speed
    ) {}
};

class Harpoon : public LightGun {
 public:
    explicit Harpoon(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::UTILITY,    // gun category
                1,                       // magazine size
                0.83/1_s,                // rof
                16_s,                    // reload
                70_hp,                   // prim dmg
                DmgType::FLECHETTE,      // prim dmg type
                60_hp,                   // sec dmg
                DmgType::PIERCING,       // sec dmg type
                1_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                800_m,                   // range
                120_m/1_s,               // projectile speed
                6_m_s2,                  // shell_drop
                0_deg,                   // jitter
                60_deg,                  // left turn angle
                60_deg,                  // right turn angle
                25_deg,                  // upwards turn angle
                40_deg,                  // downwards turn angle
                80_deg/1_s,              // horizontal turn speed
                60_deg/1_s               // vertical turn speed
    ) {}
};

class Mercury : public LightGun {
 public:
    explicit Mercury(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::OTHERS,     // gun category
                2,                       // magazine size
                0.5/1_s,                 // rof
                6_s,                     // reload
                75_hp,                   // prim dmg
                DmgType::PIERCING,       // prim dmg type
                300_hp,                  // sec dmg
                DmgType::SHATTER,        // sec dmg type
                1.25_m,                  // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                2250_m,                  // range
                750_m/1_s,               // projectile speed
                15_m_s2,                 // shell_drop
                0_deg,                   // jitter
                15_deg,                  // left turn angle
                15_deg,                  // right turn angle
                15_deg,                  // upwards turn angle
                5_deg,                   // downwards turn angle
                35_deg/1_s,              // horizontal turn speed
                35_deg/1_s               // vertical turn speed
    ) {}
};

class MineLauncher : public LightGun {
 public:
    explicit MineLauncher(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::OTHERS,     // gun category
                1,                       // magazine size
                0.25/1_s,                // rof
                4_s,                     // reload
                125_hp,                  // prim dmg
                DmgType::PIERCING,       // prim dmg type
                100_hp,                  // sec dmg
                DmgType::IMPACT,         // sec dmg type
                60_m,                    // aoe radius
                3_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0.25,                    // sec ign chance
                5,                       // sec ign stacks
                165_m,                   // range
                55_m/1_s,                // projectile speed
                13_m_s2,                 // shell_drop
                1_deg,                   // jitter
                60_deg,                  // left turn angle
                60_deg,                  // right turn angle
                45_deg,                  // upwards turn angle
                15_deg,                  // downwards turn angle
                30_deg/1_s,              // horizontal turn speed
                25_deg/1_s               // vertical turn speed
    ) {}
};

class Mortar : public LightGun {
 public:
    explicit Mortar(const std::string& name) : LightGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::MORTARS,    // gun category
                12,                      // magazine size
                1.82/1_s,                // rof
                6_s,                     // reload
                30_hp,                   // prim dmg
                DmgType::EXPLOSIVE,       // prim dmg type
                60_hp,                   // sec dmg
                DmgType::EXPLOSIVE,      // sec dmg type
                8_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                400_m,                   // range
                125_m/1_s,               // projectile speed
                7_m_s2,                  // shell_drop
                3_deg,                   // jitter
                40_deg,                  // left turn angle
                40_deg,                  // right turn angle
                30_deg,                  // upwards turn angle
                50_deg,                  // downwards turn angle
                30_deg/1_s,              // horizontal turn speed
                60_deg/1_s               // vertical turn speed
    ) {}
};

class Gatling : public LightGun {
 public:
    explicit Gatling(const std::string& name) : LightGun(
                name,
                GunType::RAYCAST,        // gun type
                GunCategory::OTHERS,     // gun category
                82,                      // magazine size
                8.33/1_s,                // rof
                5_s,                     // reload
                7.5_hp,                  // prim dmg
                DmgType::PIERCING,       // prim dmg type
                10_hp,                   // sec dmg
                DmgType::SHATTER,        // sec dmg type
                0_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                450_m,                   // range
                500_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                2.25_deg,                // jitter
                50_deg,                  // left turn angle
                50_deg,                  // right turn angle
                25_deg,                  // upwards turn angle
                50_deg,                  // downwards turn angle
                90_deg/1_s,              // horizontal turn speed
                100_deg/1_s              // vertical turn speed
    ) {}
};

class HeavyCaro : public HeavyGun {
 public:
    explicit HeavyCaro(const std::string& name) : HeavyGun(
                name,
                GunType::RAYCAST,        // gun type
                GunCategory::CARRONADES,  // gun category
                2,                       // magazine size
                0.83/1_s,                // rof
                7.5_s,                   // reload
                350_hp,                  // prim dmg
                DmgType::FLECHETTE,      // prim dmg type
                200_hp,                  // sec dmg
                DmgType::SHATTER,        // sec dmg type
                0_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                400_m,                   // range
                800_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                6_deg,                   // jitter
                25_deg,                  // left turn angle
                25_deg,                  // right turn angle
                20_deg,                  // upwards turn angle
                10_deg,                  // downwards turn angle
                60_deg/1_s,              // horizontal turn speed
                50_deg/1_s               // vertical turn speed
    ) {}
};

class Hwacha : public HeavyGun {
 public:
    explicit Hwacha(const std::string& name) : HeavyGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::ROCKET_LAUNCHERS,  // gun category
                20,                      // magazine size
                8.33/1_s,                // rof
                14_s,                    // reload
                25_hp,                   // prim dmg
                DmgType::EXPLOSIVE,      // prim dmg type
                45_hp,                   // sec dmg
                DmgType::SHATTER,        // sec dmg type
                7_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                1200_m,                  // range
                250_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                4_deg,                   // jitter
                30_deg,                  // left turn angle
                30_deg,                  // right turn angle
                20_deg,                  // upwards turn angle
                20_deg,                  // downwards turn angle
                55_deg/1_s,              // horizontal turn speed
                35_deg/1_s               // vertical turn speed
    ) {}
};

class Lumberjack : public HeavyGun {
 public:
    explicit Lumberjack(const std::string& name) : HeavyGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::MORTARS,    // gun category
                6,                       // magazine size
                0.8/1_s,                 // rof
                8_s,                     // reload
                50_hp,                   // prim dmg
                DmgType::SHATTER,        // prim dmg type
                300_hp,                  // sec dmg
                DmgType::FLECHETTE,      // sec dmg type
                12_m,                    // aoe radius
                0.8_s,                   // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                1650_m,                  // range
                275_m/1_s,               // projectile speed
                20_m_s2,                 // shell_drop
                0_deg,                   // jitter
                15_deg,                  // left turn angle
                15_deg,                  // right turn angle
                65_deg,                  // upwards turn angle
                25_deg,                  // downwards turn angle
                15_deg/1_s,              // horizontal turn speed
                25_deg/1_s               // vertical turn speed
    ) {}
};

class HeavyFlak : public HeavyGun {
 public:
    explicit HeavyFlak(const std::string& name) : HeavyGun(
                name,
                GunType::PROJECTILE,     // gun type
                GunCategory::FLAKS,      // gun category
                4,                       // magazine size
                2/1_s,                   // rof
                4.5_s,                   // reload
                115_hp,                  // prim dmg
                DmgType::EXPLOSIVE,      // prim dmg type
                50_hp,                   // sec dmg
                DmgType::EXPLOSIVE,      // sec dmg type
                7_m,                     // aoe radius
                1.25_s,                  // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0.2,                     // sec ign chance
                4,                       // sec ign stacks
                1680_m,                  // range
                240_m/1_s,               // projectile speed
                6_m_s2,                  // shell_drop
                0_deg,                   // jitter
                30_deg,                  // left turn angle
                30_deg,                  // right turn angle
                30_deg,                  // upwards turn angle
                40_deg,                  // downwards turn angle
                30_deg/1_s,              // horizontal turn speed
                20_deg/1_s               // vertical turn speed
    ) {}
};

class Minotaur : public HeavyGun {
 public:
    explicit Minotaur(const std::string& name) : HeavyGun(
                name,
                GunType::RAYCAST,        // gun type
                GunCategory::OTHERS,     // gun category
                4,                       // magazine size
                0.33/1_s,                // rof
                10_s,                    // reload
                90_hp,                   // prim dmg
                DmgType::PIERCING,       // prim dmg type
                225_hp,                  // sec dmg
                DmgType::SHATTER,        // sec dmg type
                0_m,                     // aoe radius
                0_s,                     // arming time
                0,                       // prim ign chance
                1,                       // prim ign stacks
                0,                       // sec ign chance
                1,                       // sec ign stacks
                1800_m,                  // range
                550_m/1_s,               // projectile speed
                0_m_s2,                  // shell_drop
                3.5_deg,                 // jitter
                30_deg,                  // left turn angle
                30_deg,                  // right turn angle
                30_deg,                  // upwards turn angle
                30_deg,                  // downwards turn angle
                20_deg/1_s,              // horizontal turn speed
                20_deg/1_s               // vertical turn speed
    ) {}
};

}  //  namespace goio

#endif  // GUNS_H_
