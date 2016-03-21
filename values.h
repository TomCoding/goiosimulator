#include <cmath>
#include <map>
#include <functional>
#include <string>

namespace goio {
  typedef std::function<double ()> TimeFunc;
  typedef std::function<TimeFunc ()> TimeCheckFunc;

  enum DmgType {
    FIRE      = 0,
    FLECHETTE = 1,
    SHATTER   = 2,
    PIERCING  = 3,
    EXPLOSIVE = 4,
    IMPACT    = 5
  };
  static const std::string DmgTypeString[] {"fire",
                                            "flechette",
                                            "shatter",
                                            "piercing",
                                            "explosive",
                                            "impact"};
  inline const std::string get_dmg_type_string(DmgType val) { return DmgTypeString[val]; }

  enum CmpType {
    BALLOON    = 0,
    HULL       = 1,
    ARMOR      = 2,
    COMPONENTS = 3
  };
  static const std::string CmpTypeString[] {"balloon",
                                            "hull",
                                            "armor",
                                            "component"};
  inline const std::string get_cmp_type_string(CmpType val) { return CmpTypeString[val]; }

  static const double dmg_types[6][4] = {
    // balloon  hull  armor  components
      {  1.5,   1.3,   0.8,  0.25  }, // fire
      {  1.8,   0.2,   0.25, 0     }, // flechette
      {  0.2,   0.1,   0.2,  2     }, // shatter
      {  0.2,   0.2,   1.5,  0.2   }, // piercing
      {  0.25,  1.4,   0.3,  0.3   }, // explosive
      {  1.8,   1.5,   0.8,  0.6   }, // impact
  };

  class GoioObj {
    private:
      std::string name;
      CmpType cmp_type;
      double max_health;
      double health;
      GoioObj* hull;

      GoioObj() : name(""), cmp_type(CmpType::HULL), max_health(0), health(0), hull(nullptr) {}

    public:
      GoioObj(std::string name, CmpType cmp_type, double max_health = 0, double hull_max_health = 0);
      ~GoioObj();

      inline std::string get_name() { return name; }
      inline CmpType get_cmp_type() { return cmp_type; }
      inline double  get_max_health() { return max_health; }
      inline double  get_health() { return health; }
      inline GoioObj* get_hull() { return hull; }

      /*
       * Return `false` if object gets destroyed, otherwise `true`
       */
      bool add_health(double health);

      void reset();
      void reset_all();
  };

  typedef std::function<bool (GoioObj*)> TimeDmgFunc;

  class Ship : public GoioObj {
    public:
      Ship(std::string name, double max_health, double hull_max_health) :
                GoioObj(name, CmpType::ARMOR, max_health, hull_max_health) {};
  };

  class Pyramidion : public Ship {
    public:
      Pyramidion(std::string name) : Ship(name, 650, 700) {}
  };

  class Goldfish : public Ship {
    public:
      Goldfish(std::string name) : Ship(name, 400, 1100) {}
  };

  class Junker : public Ship {
    public:
      Junker(std::string name) : Ship(name, 700, 500) {}
  };

  class Galleon : public Ship {
    public:
      Galleon(std::string name) : Ship(name, 800, 1400) {}
  };

  class Squid : public Ship {
    public:
      Squid(std::string name) : Ship(name, 230, 950) {}
  };

  class Spire : public Ship {
    public:
      Spire(std::string name) : Ship(name, 400, 950) {}
  };

  class Mobula : public Ship {
    public:
      Mobula(std::string name) : Ship(name, 600, 700) {}
  };

  class Ammunition {
    private:
      double clipsize;
      double damage;
      bool   proportional_self_damage;

    protected:
      Ammunition(double clipsize,
                 double damage,
                 bool   proportional_self_damage) : clipsize(clipsize),
                                damage(damage),
                                proportional_self_damage(proportional_self_damage) {}

    public:
      inline double get_clipsize() { return clipsize; }
      inline double get_damage() { return damage; }
      inline bool   get_proportional_self_damage() { return proportional_self_damage; }
  };

  class Lochnagar : public Ammunition {
    public:
      Lochnagar() : Ammunition(
                      0.4,
                      2.25,
                      true
      ) {};
  };


  class GunInfo {
    private:
      int     clipsize;
      double  rof;
      double  reload_;
      double  direct_dmg;
      DmgType direct_dmg_type;
      double  aoe_dmg;
      DmgType aoe_dmg_type;
      double  aoe_radius;

      int     cur_clipsize;
      double  cur_rof;
      double  cur_reload;
      double  cur_direct_dmg;
      DmgType cur_direct_dmg_type;
      double  cur_aoe_dmg;
      DmgType cur_aoe_dmg_type;
      double  cur_aoe_radius;
      Ammunition* cur_ammo;

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
                                   aoe_radius(aoe_radius),
                                   cur_clipsize(clipsize),
                                   cur_rof(rof),
                                   cur_reload(reload),
                                   cur_direct_dmg(direct_dmg),
                                   cur_direct_dmg_type(direct_dmg_type),
                                   cur_aoe_dmg(aoe_dmg),
                                   cur_aoe_dmg_type(aoe_dmg_type),
                                   cur_aoe_radius(aoe_radius),
                                   cur_ammo(nullptr) {}

      void set_clipsize(double clipsize);
      void set_rof(double rof);
      void set_reload(double reload_);
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

    public:
      inline int     get_max_clipsize() { return clipsize; }
      inline int     get_clipsize() { return cur_clipsize; }
      inline double  get_rof() { return cur_rof; }
      inline double  get_reload() { return cur_reload; }
      inline double  get_direct_dmg() { return cur_direct_dmg; }
      inline DmgType get_direct_dmg_type() { return cur_direct_dmg_type; }
      inline double  get_aoe_dmg() { return cur_aoe_dmg; }
      inline DmgType get_aoe_dmg_type() { return cur_aoe_dmg_type; }
      inline double  get_aoe_radius() { return cur_aoe_radius; }
      inline Ammunition* get_ammo() { return cur_ammo; }

      void apply_ammunition(Ammunition* ammo);

      void reload(bool with_ammo = true);
  };

  class Gun : public GunInfo, public GoioObj {
    protected:
      Gun(std::string name, double max_health,
          int clipsize, double rof, double reload, double direct_dmg,
          DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type,
          double aoe_radius) :
                  GunInfo(clipsize, rof, reload, direct_dmg, direct_dmg_type,
                          aoe_dmg, aoe_dmg_type, aoe_radius),
                  GoioObj(name, CmpType::COMPONENTS, max_health) {}

    public:
      inline double get_rof_changed() { return get_health()/get_max_health()*get_rof(); }
      inline double get_time_per_shot() { return get_max_health()/get_health()/get_rof(); }
      inline double get_reload_changed() { return get_max_health()/get_health()*get_reload(); }

      bool shoot(GoioObj* obj, bool aoe, double aoe_range);
      inline bool shoot(GoioObj* obj) { return shoot(obj, true, 0); }

      TimeFunc get_time_func();
  };

  class LightGun : public Gun {
    protected:
      LightGun(std::string name, int clipsize, double rof, double reload,
               double direct_dmg, DmgType direct_dmg_type, double aoe_dmg,
               DmgType aoe_dmg_type, double aoe_radius) :
                  Gun(name, 250, clipsize, rof, reload, direct_dmg, direct_dmg_type,
                      aoe_dmg, aoe_dmg_type, aoe_radius) {};
  };

  class Gatling : public LightGun {
    public:
      Gatling(std::string name = "") : LightGun(
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

  class TimeObj {
    private:
      double time;
      struct FuncData {
        GoioObj* registrar;
        TimeDmgFunc timedmgfunc;
        GoioObj* obj;
        double time_prev;
        double time_next;
        double time_cur;
        TimeCheckFunc timecheckfunc;
      };

      std::multimap<double, FuncData*> events;
      std::multimap<GoioObj*, FuncData*> registrars;

      bool register_event(FuncData* funcdata, double time);
      bool recalc_next_event(FuncData* funcdata);

    public:
      TimeObj() : time(0), events(), registrars() {}
      ~TimeObj();

      inline double get_time() { return time; }
      bool next_event();
      bool register_event(GoioObj* registrar, TimeDmgFunc timedmgfunc,
                          GoioObj* obj, TimeCheckFunc timecheckfunc,
                          double time = 0, bool rel = true);

      void reset();
  };

}
