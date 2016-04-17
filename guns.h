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
    const int     clipsize;
    const double  rof;
    const double  reload_;
    const double  direct_dmg;
    const DmgType direct_dmg_type;
    const double  aoe_dmg;
    const DmgType aoe_dmg_type;
    const double  aoe_radius;
    const double  direct_ign_chance;
    const int     direct_ign_stacks;
    const double  aoe_ign_chance;
    const int     aoe_ign_stacks;

 protected:
    GunInfo(int clipsize,
            double rof,
            double reload,
            double direct_dmg,
            DmgType direct_dmg_type,
            double aoe_dmg,
            DmgType aoe_dmg_type,
            double aoe_radius,
            double direct_ign_chance,
            int direct_ign_stacks,
            double aoe_ign_chance,
            int aoe_ign_stacks) : clipsize(clipsize),
                                  rof(rof),
                                  reload_(reload),
                                  direct_dmg(direct_dmg),
                                  direct_dmg_type(direct_dmg_type),
                                  aoe_dmg(aoe_dmg),
                                  aoe_dmg_type(aoe_dmg_type),
                                  aoe_radius(aoe_radius),
                                  direct_ign_chance(direct_ign_chance),
                                  direct_ign_stacks(direct_ign_stacks),
                                  aoe_ign_chance(aoe_ign_chance),
                                  aoe_ign_stacks(aoe_ign_stacks) {}
    virtual ~GunInfo() {}

 public:
    inline int     get_orig_clipsize() const { return clipsize; }
    inline double  get_orig_rof() const { return rof; }
    inline double  get_orig_reload() const { return reload_; }
    inline double  get_orig_direct_dmg() const { return direct_dmg; }
    inline DmgType get_orig_direct_dmg_type() const { return direct_dmg_type; }
    inline double  get_orig_aoe_dmg() const { return aoe_dmg; }
    inline DmgType get_orig_aoe_dmg_type() const { return aoe_dmg_type; }
    inline double  get_orig_aoe_radius() const { return aoe_radius; }
    inline double  get_orig_direct_ign_chance() const { return direct_ign_chance; }
    inline int     get_orig_direct_ign_stacks() const { return direct_ign_stacks; }
    inline double  get_orig_aoe_ign_chance() const { return aoe_ign_chance; }
    inline int     get_orig_aoe_ign_stacks() const { return aoe_ign_stacks; }
};

class Gun : public GunInfo, public ShootActor {
 private:
    int     cur_clipsize;
    double  cur_rof;
    double  cur_reload;
    double  cur_direct_dmg;
    DmgType cur_direct_dmg_type;
    double  cur_aoe_dmg;
    DmgType cur_aoe_dmg_type;
    double  cur_aoe_radius;
    double  cur_direct_ign_chance;
    double  cur_aoe_ign_chance;

    const Ammunition* cur_ammo;

    bool during_reload;

    Gun(const Gun& obj);
    Gun& operator=(const Gun& obj);

    void set_clipsize(double clipsize);
    void set_rof(double rof);
    void set_reload(double reload);
    void set_direct_dmg(double direct_dmg);
    void set_direct_dmg_type(DmgType direct_dmg_type);
    void set_aoe_dmg(double aoe_dmg);
    void set_aoe_dmg_type(DmgType aoe_dmg_type);
    void set_aoe_radius(double aoe_radius);
    void set_direct_ign_chance(double ign_chance);
    void set_aoe_ign_chance(double ign_chance);

    inline int dec_clipsize() {
      if (--cur_clipsize < 0)
        cur_clipsize = 0;
      return cur_clipsize;
    }

 protected:
    Gun(const std::string& name, double max_health,
        int clipsize, double rof, double reload, double direct_dmg,
        DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type,
        double aoe_radius, double direct_ign_chance, int direct_ign_stacks,
        double aoe_ign_chance, int aoe_ign_stacks) :
                GunInfo(clipsize, rof, reload, direct_dmg, direct_dmg_type,
                        aoe_dmg, aoe_dmg_type, aoe_radius, direct_ign_chance,
                        direct_ign_stacks, aoe_ign_chance, aoe_ign_stacks),
                ShootActor(name, CmpType::COMPONENTS, 2.33, max_health),
                cur_clipsize(clipsize),
                cur_rof(rof),
                cur_reload(reload),
                cur_direct_dmg(direct_dmg),
                cur_direct_dmg_type(direct_dmg_type),
                cur_aoe_dmg(aoe_dmg),
                cur_aoe_dmg_type(aoe_dmg_type),
                cur_aoe_radius(aoe_radius),
                cur_direct_ign_chance(direct_ign_chance),
                cur_aoe_ign_chance(aoe_ign_chance),
                cur_ammo(nullptr),
                during_reload(false) {}

 public:
    virtual ~Gun() {}

    int    get_max_clipsize() const;
    double get_max_rof() const;
    inline int     get_clipsize() const { return cur_clipsize; }
    inline double  get_rof() const { return cur_rof; }
    inline double  get_reload() const { return cur_reload; }
    inline double  get_direct_dmg() const { return cur_direct_dmg; }
    inline DmgType get_direct_dmg_type() const { return cur_direct_dmg_type; }
    inline double  get_aoe_dmg() const { return cur_aoe_dmg; }
    inline DmgType get_aoe_dmg_type() const { return cur_aoe_dmg_type; }
    inline double  get_aoe_radius() const { return cur_aoe_radius; }
    inline double  get_direct_ign_chance() const { return cur_direct_ign_chance; }
    inline double  get_aoe_ign_chance() const { return cur_aoe_ign_chance; }
    inline const Ammunition* get_ammo() const { return cur_ammo; }

    bool apply_ammunition(const Ammunition* ammo);

    void reload(bool with_ammo = true);

    double get_rof_changed() const;
    double get_time_per_shot() const;
    double get_reload_changed() const;

    DmgState::State shoot(GoioObj* obj, double,
                          bool aoe, double aoe_range) override;

    TimeFunc get_time_func(const GoioObj*, double, bool&) override;
};

class LightGun : public Gun {
 protected:
    LightGun(const std::string& name, int clipsize, double rof, double reload,
             double direct_dmg, DmgType direct_dmg_type, double aoe_dmg,
             DmgType aoe_dmg_type, double aoe_radius, double direct_ign_chance,
             int direct_ign_stacks, double aoe_ign_chance, int aoe_ign_stacks) :
                Gun(name, 250, clipsize, rof, reload,
                    direct_dmg, direct_dmg_type,
                    aoe_dmg, aoe_dmg_type, aoe_radius, direct_ign_chance,
                    direct_ign_stacks, aoe_ign_chance, aoe_ign_stacks) {}

 public:
    ~LightGun() {}
};

class Artemis : public LightGun {
 public:
    explicit Artemis(const std::string& name) : LightGun(
                    name,
                    4,                   // magazine size
                    0.63,                // rof
                    7,                   // reload
                    70,                  // prim dmg
                    DmgType::EXPLOSIVE,  // prim dmg type
                    120,                 // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    2.5,                 // aoe radius
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1                    // sec ign stacks
    ) {}
};

class Banshee : public LightGun {
 public:
    explicit Banshee(const std::string& name) : LightGun(
                    name,
                    8,                   // magazine size
                    2,                   // rof
                    6,                   // reload
                    25,                  // prim dmg
                    DmgType::EXPLOSIVE,  // prim dmg type
                    25,                  // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    3,                   // aoe radius
                    0.35,                // prim ign chance
                    1,                   // prim ign stacks
                    0.264,               // sec ign chance
                    2                    // sec ign stacks
    ) {}
};

class LightCaro : public LightGun {
 public:
    explicit LightCaro(const std::string& name) : LightGun(
                    name,
                    5,                   // magazine size
                    1,                   // rof
                    6,                   // reload
                    96,                  // prim dmg
                    DmgType::FLECHETTE,  // prim dmg type
                    144,                 // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    0,                   // aoe radius
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1                    // sec ign stacks
    ) {}
};

class Flare : public LightGun {
 public:
    explicit Flare(const std::string& name) : LightGun(
                    name,
                    2,                   // magazine size
                    0.5,                 // rof
                    20,                  // reload
                    10,                  // prim dmg
                    DmgType::FIRE,       // prim dmg type
                    5,                   // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    3,                   // aoe radius
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1                    // sec ign stacks
    ) {}
};

class Flamethrower : public LightGun {
 public:
    explicit Flamethrower(const std::string& name) : LightGun(
                    name,
                    250,                 // magazine size
                    16.67,               // rof
                    6,                   // reload
                    1.2,                 // prim dmg
                    DmgType::FIRE,       // prim dmg type
                    0,                   // sec dmg
                    DmgType::FIRE,       // sec dmg type
                    4,                   // aoe radius
                    0.22,                // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1                    // sec ign stacks
    ) {}
};

class Gatling : public LightGun {
 public:
    explicit Gatling(const std::string& name) : LightGun(
                    name,
                    82,                  // magazine size
                    8.33,                // rof
                    5,                   // reload
                    7.5,                 // prim dmg
                    DmgType::PIERCING,   // prim dmg type
                    10,                  // sec dmg
                    DmgType::SHATTER,    // sec dmg type
                    0,                   // aoe radius
                    0,                   // prim ign chance
                    1,                   // prim ign stacks
                    0,                   // sec ign chance
                    1                    // sec ign stacks
    ) {}
};

}  //  namespace goio

#endif  // GUNS_H_
