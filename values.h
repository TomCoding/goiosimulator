#include <cmath>

namespace goio {

  enum DmgType {
    FIRE      = 0,
    FLECHETTE = 1,
    SHATTER   = 2,
    PIERCING  = 3,
    EXPLOSIVE = 4,
    IMPACT    = 5
  };

  enum CmpType {
    BALLOON    = 0,
    HULL       = 1,
    ARMOR      = 2,
    COMPONENTS = 3
  };

  static const double dmg_types[6][4] = {
    {1.5,  1.3, 0.8,  0.25}, // fire
    {1.8,  0.2, 0.25, 0},    // flechette
    {0.2,  0.1, 0.2,  2},    // shatter
    {0.2,  0.2, 1.5,  0.2},  // piercing
    {0.25, 1.4, 0.3,  0.3},  // explosive
    {1.8,  1.5, 0.8,  0.6},  // impact
  };

  class GoioObj {
    private:
      CmpType cmp_type;
      double max_health;
      double health;
      GoioObj* hull;

      GoioObj() {};

    public:
      GoioObj(CmpType cmp_type, double max_health = 0, double hull_max_health = 0);
      ~GoioObj();

      inline CmpType get_cmp_type() { return cmp_type; };
      inline double  get_max_health() { return max_health; };
      inline double  get_health() { return health; };
      inline GoioObj* get_hull() { return hull; };

      /*
       * Return `false` if object gets destroyed, otherwise `true`
       */
      bool add_health(double health);

      void reset();
      void reset_all();
  };

  class Ammunition {
    private:
      double clipsize;
      double damage;
      bool   proportional_self_damage;

    protected:
      Ammunition(double clipsize,
                 double damage,
                 bool   proportional_self_damage);

    public:
      inline double get_clipsize() { return clipsize; }
      inline double get_damage() { return damage; }
      inline bool   get_proportional_self_damage() { return proportional_self_damage; }
  };

  class Lochnagar : public Ammunition {
    public:
      Lochnagar() : Ammunition(
                      0.5,
                      2.25,
                      true
      ) {};
  };


  class GunInfo {
    private:
      int     clipsize;
      double  rof;
      double  reload;
      double  direct_dmg;
      DmgType direct_dmg_type;
      double  aoe_dmg;
      DmgType aoe_dmg_type;

      int     cur_clipsize;
      double  cur_rof;
      double  cur_reload;
      double  cur_direct_dmg;
      DmgType cur_direct_dmg_type;
      double  cur_aoe_dmg;
      DmgType cur_aoe_dmg_type;
      Ammunition* cur_ammo;

    protected:
      GunInfo(int clipsize,
              double rof,
              double reload,
              double direct_dmg,
              DmgType direct_dmg_type,
              double aoe_dmg,
              DmgType aoe_dmg_type);
      void set_clipsize(double clipsize);
      void set_rof(double rof);
      void set_reload(double cur_reload);
      void set_direct_dmg(double cur_direct_dmg);
      void set_direct_dmg_type(DmgType cur_direct_dmg_type);
      void set_aoe_dmg(double cur_aoe_dmg);
      void set_aoe_dmg_type(DmgType cur_aoe_dmg_type);

      inline int dec_clipsize() {
        if (--cur_clipsize < 0)
          cur_clipsize = 0;
        return cur_clipsize;
      }

    public:
      inline int     get_max_clipsize() { return clipsize; }
      inline int     get_clipsize() { return cur_clipsize; }
      inline double  get_rof() { return cur_rof; }
      inline double  get_reload() { return cur_reload; }
      inline double  get_direct_dmg() { return cur_direct_dmg; }
      inline DmgType get_direct_dmg_type() { return cur_direct_dmg_type; }
      inline double  get_aoe_dmg() { return cur_aoe_dmg; }
      inline DmgType get_aoe_dmg_type() { return cur_aoe_dmg_type; }
      inline Ammunition* get_ammo() { return cur_ammo; }

      void apply_ammunition(Ammunition* ammo);

      void reset();
  };

  class Gun : public GunInfo {
    private:
      double health;
      double max_health;

    protected:
      Gun(double max_health,
          int clipsize, double rof, double reload, double direct_dmg,
          DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type);
      void add_health(double health);

    public:
      inline double get_rof_changed() { return max_health/health*get_rof(); }
      inline double get_health() { return health; }
      inline double get_max_health() { return max_health; }

      double shoot(GoioObj* obj, bool aoe = true, double aoe_range = 0);
  };

  class LightGun : public Gun {
    protected:
      LightGun(int clipsize, double rof, double reload, double direct_dmg,
               DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type) :
                  Gun(250, clipsize, rof, reload, direct_dmg, direct_dmg_type,
                      aoe_dmg, aoe_dmg_type) {};
  };

  class Gatling : public LightGun {
    public:
      Gatling() : LightGun(
                      82,
                      8.33,
                      5,
                      7.5,
                      DmgType::PIERCING,
                      10,
                      DmgType::SHATTER
      ) {};
  };

}
