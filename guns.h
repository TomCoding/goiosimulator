#ifndef __GUNS_H
#define __GUNS_H

#include "goioobj.h"
#include "goioactor.h"
#include "dmg_types.h"
#include "ammunitions.h"
#include <string>

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

    protected:
      GunInfo(int clipsize,
              double rof,
              double reload,
              double direct_dmg,
              DmgType direct_dmg_type,
              double aoe_dmg,
              DmgType aoe_dmg_type,
              double aoe_radius) : clipsize(clipsize),
                                   rof(rof),
                                   reload_(reload),
                                   direct_dmg(direct_dmg),
                                   direct_dmg_type(direct_dmg_type),
                                   aoe_dmg(aoe_dmg),
                                   aoe_dmg_type(aoe_dmg_type),
                                   aoe_radius(aoe_radius) {}
      virtual ~GunInfo() {};

    public:
      inline int     get_orig_clipsize() const { return clipsize; }
      inline double  get_orig_rof() const { return rof; }
      inline double  get_orig_reload() const { return reload_; }
      inline double  get_orig_direct_dmg() const { return direct_dmg; }
      inline DmgType get_orig_direct_dmg_type() const { return direct_dmg_type; }
      inline double  get_orig_aoe_dmg() const { return aoe_dmg; }
      inline DmgType get_orig_aoe_dmg_type() const { return aoe_dmg_type; }
      inline double  get_orig_aoe_radius() const { return aoe_radius; }
  };

  class Gun : public GunInfo, public GoioActor {
    private:
      int     cur_clipsize;
      double  cur_rof;
      double  cur_reload;
      double  cur_direct_dmg;
      DmgType cur_direct_dmg_type;
      double  cur_aoe_dmg;
      DmgType cur_aoe_dmg_type;
      double  cur_aoe_radius;

      const Ammunition* cur_ammo;

      bool during_reload;

      Gun(const Gun& obj) : GunInfo(obj.get_orig_clipsize(), obj.get_orig_rof(),
                  obj.get_orig_reload(), obj.get_orig_direct_dmg(),
                  obj.get_orig_direct_dmg_type(), obj.get_orig_aoe_dmg(),
                  obj.get_orig_aoe_dmg_type(), obj.get_orig_aoe_radius()),
              GoioActor(obj.get_name(), CmpType::COMPONENTS, 2.33, obj.get_max_health()),
              cur_clipsize(obj.cur_clipsize), cur_rof(obj.cur_rof), cur_reload(obj.cur_reload),
              cur_direct_dmg(obj.cur_direct_dmg), cur_direct_dmg_type(obj.cur_direct_dmg_type),
              cur_aoe_dmg(obj.cur_aoe_dmg), cur_aoe_dmg_type(obj.cur_aoe_dmg_type),
              cur_aoe_radius(obj.cur_aoe_radius), cur_ammo(obj.cur_ammo),
              during_reload(false) {}
      Gun& operator=(const Gun&) { return *this; };

      void set_clipsize(double clipsize);
      void set_rof(double rof);
      void set_reload(double reload);
      void set_direct_dmg(double direct_dmg);
      void set_direct_dmg_type(DmgType direct_dmg_type);
      void set_aoe_dmg(double aoe_dmg);
      void set_aoe_dmg_type(DmgType aoe_dmg_type);
      void set_aoe_radius(double aoe_radius);

      inline int dec_clipsize() {
        if (--cur_clipsize < 0)
          cur_clipsize = 0;
        return cur_clipsize;
      }

    protected:
      Gun(const std::string name, double max_health,
          int clipsize, double rof, double reload, double direct_dmg,
          DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type,
          double aoe_radius) :
                  GunInfo(clipsize, rof, reload, direct_dmg, direct_dmg_type,
                          aoe_dmg, aoe_dmg_type, aoe_radius),
                  GoioActor(name, CmpType::COMPONENTS, 2.33, max_health),
                  cur_clipsize(clipsize),
                  cur_rof(rof),
                  cur_reload(reload),
                  cur_direct_dmg(direct_dmg),
                  cur_direct_dmg_type(direct_dmg_type),
                  cur_aoe_dmg(aoe_dmg),
                  cur_aoe_dmg_type(aoe_dmg_type),
                  cur_aoe_radius(aoe_radius),
                  cur_ammo(nullptr),
                  during_reload(false) {}

    public:
      virtual ~Gun() {}

      int get_max_clipsize() const;
      inline int     get_clipsize() const { return cur_clipsize; }
      inline double  get_rof() const { return cur_rof; }
      inline double  get_reload() const { return cur_reload; }
      inline double  get_direct_dmg() const { return cur_direct_dmg; }
      inline DmgType get_direct_dmg_type() const { return cur_direct_dmg_type; }
      inline double  get_aoe_dmg() const { return cur_aoe_dmg; }
      inline DmgType get_aoe_dmg_type() const { return cur_aoe_dmg_type; }
      inline double  get_aoe_radius() const { return cur_aoe_radius; }
      inline const Ammunition* get_ammo() const { return cur_ammo; }

      void apply_ammunition(const Ammunition* ammo);

      void reload(bool with_ammo = true);

      inline double get_rof_changed() const { return get_health()/get_max_health()*get_rof(); }
      inline double get_time_per_shot() const { return get_max_health()/get_health()/get_rof(); }
      inline double get_reload_changed() const { return get_max_health()/get_health()*get_reload(); }

      bool shoot(GoioObj* obj, bool& changed, bool aoe, double aoe_range);
      inline bool shoot(GoioObj* obj, bool& changed) { return shoot(obj, changed, true, 0); }

      TimeFunc get_time_func(const GoioObj*, double, bool&) override;
  };

  class LightGun : public Gun {
    protected:
      LightGun(const std::string name, int clipsize, double rof, double reload,
               double direct_dmg, DmgType direct_dmg_type, double aoe_dmg,
               DmgType aoe_dmg_type, double aoe_radius) :
                  Gun(name, 250, clipsize, rof, reload, direct_dmg, direct_dmg_type,
                      aoe_dmg, aoe_dmg_type, aoe_radius) {};

    public:
      ~LightGun() {}
  };

  class Gatling : public LightGun {
    public:
      explicit Gatling(const std::string name = "") : LightGun(
                      name,
                      82,
                      8.33,
                      5,
                      7.5,
                      DmgType::PIERCING,
                      10,
                      DmgType::SHATTER,
                      0
      ) {};
  };

}

#endif // __GUNS_H
