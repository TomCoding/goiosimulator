#include <cmath>

namespace goio {

  enum DmgType {
    PIERCING,
    SHATTER,
    EXPLOSIVE,
    IMPACT,
    FLECHETTE
  };

  // class GoioObj {
  //   private:
  //     double time;
  //
  //   public:
  //     GoioObj() : time(0) {};
  //     double get_time() { return time; }
  // };


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
      inline bool   proportional_get_self_damage() { return proportional_self_damage; }
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

    public:
      inline int get_clipsize() { return cur_clipsize; }
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
      void add_health(double add);

    public:
      inline double get_rof_changed() { return max_health/health; }
      inline double get_health() { return health; }
      inline double get_max_health() { return max_health; }
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
