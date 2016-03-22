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
      const std::string name;
      const CmpType cmp_type;
      const double max_health;
      double health;
      int rebuild_state;
      const int part_type_multiplier;
      GoioObj* hull;

      GoioObj(double max_health) : name(""), cmp_type(CmpType::HULL),
                  max_health(max_health), health(max_health), rebuild_state(-1),
                  part_type_multiplier(-1), hull(nullptr) {}
      GoioObj(const GoioObj& obj) : name(obj.name), cmp_type(obj.cmp_type),
                  max_health(obj.max_health), health(obj.max_health),
                  rebuild_state(obj.rebuild_state), part_type_multiplier(obj.part_type_multiplier),
                  hull(obj.hull) {}
      GoioObj& operator=(const GoioObj&) { return *this; };

    public:
      GoioObj(const std::string name, CmpType cmp_type, int part_type_multiplier = -1,
              double max_health = 0, double hull_max_health = 0) :
              name(name), cmp_type(cmp_type), max_health(max_health), health(max_health),
              rebuild_state(-1), part_type_multiplier(part_type_multiplier),
              hull(new GoioObj(hull_max_health)) {}
      virtual ~GoioObj();

      const int    rebuild_base_hits         = 9;
      const double rebuild_health_multiplier = 0.05;
      const double health_after_rebuild      = 0.333333333333;

      inline int get_rebuild_value() const {
        return std::round((rebuild_base_hits+max_health*rebuild_health_multiplier)*part_type_multiplier);
      }

      inline const std::string get_name() const { return name; }
      inline CmpType get_cmp_type() const { return cmp_type; }
      inline double  get_max_health() const { return max_health; }
      inline double  get_health() const { return health; }
      inline double  get_rebuild_state() const { return rebuild_state; }
      inline GoioObj* get_hull() const { return hull; }

      /*
       * Return `false` if object gets destroyed, otherwise `true`
       */
      bool add_health(double health);
      bool add_rebuild(int rebuild_progress);

      virtual void reset(bool hull = true);
  };

  typedef std::function<bool (GoioObj*)> TimeDmgFunc;

  class Ship : public GoioObj {
    public:
      Ship(const std::string name, double max_health, double hull_max_health) :
                GoioObj(name, CmpType::ARMOR, 1, max_health, hull_max_health) {};
      virtual ~Ship() {}
  };

  class Pyramidion : public Ship {
    public:
      explicit Pyramidion(const std::string name) : Ship(name, 650, 700) {}
  };

  class Goldfish : public Ship {
    public:
      explicit Goldfish(const std::string name) : Ship(name, 400, 1100) {}
  };

  class Junker : public Ship {
    public:
      explicit Junker(const std::string name) : Ship(name, 700, 500) {}
  };

  class Galleon : public Ship {
    public:
      explicit Galleon(const std::string name) : Ship(name, 800, 1400) {}
  };

  class Squid : public Ship {
    public:
      explicit Squid(const std::string name) : Ship(name, 230, 950) {}
  };

  class Spire : public Ship {
    public:
      explicit Spire(const std::string name) : Ship(name, 400, 950) {}
  };

  class Mobula : public Ship {
    public:
      explicit Mobula(const std::string name) : Ship(name, 600, 700) {}
  };


  class Ammunition {
    protected:
      const double clipsize;
      const double damage;
      const double aoe_radius;
      const bool proportional_self_damage;

      Ammunition(double clipsize, double damage, double aoe_radius,
                 bool proportional_self_damage) : clipsize(clipsize),
                        damage(damage), aoe_radius(aoe_radius),
                        proportional_self_damage(proportional_self_damage) {}

    public:
      virtual ~Ammunition() {}
      inline double get_clipsize() const { return clipsize; }
      inline double get_damage() const { return damage; }
      inline double get_aoe_radius() const { return aoe_radius; }
      inline bool   get_proportional_self_damage() const { return proportional_self_damage; }
  };

  class Lochnagar : public Ammunition {
    public:
      Lochnagar() : Ammunition(0.4,
                               2.25,
                               0.5,
                               true) {};
  };


  class RepairInfo {
    private:
      const double swing;
      const double heal;
      const int    rebuild_power;
      const int    extinguish;
      const double cooldown;

    protected:
      RepairInfo(double swing,
                 double heal,
                 int rebuild_power,
                 int extinguish,
                 double cooldown) : swing(swing),
                                    heal(heal),
                                    rebuild_power(rebuild_power),
                                    extinguish(extinguish),
                                    cooldown(cooldown) {}
      virtual ~RepairInfo() {};

    public:
      inline double get_swing() const { return swing; }
      inline double get_heal() const { return heal; }
      inline int    get_rebuild_power() const { return rebuild_power; }
      inline int    get_extinguish() const { return extinguish; }
      inline double get_cooldown() const { return cooldown; }
  };

  class RepairTool : public RepairInfo, public GoioObj {
    private:
      int done; // 0 = normal, 1 = rebuild swings, 2 = done

    protected:
      RepairTool(const std::string name, double swing, double heal,
                 int rebuild_power, int extinguish, double cooldown) :
                      RepairInfo(swing, heal, rebuild_power, extinguish, cooldown),
                      GoioObj(name, CmpType::COMPONENTS),
                      done(0) {}

    public:
      virtual ~RepairTool() {}

      bool repair(GoioObj* obj);

      TimeFunc get_time_func();
  };

  class Spanner : public RepairTool {
    public:
      explicit Spanner(const std::string name) : RepairTool(name, 0.7, 40, 5, 0, 2) {}
  };

  class Mallet : public RepairTool {
    public:
      explicit Mallet(const std::string name) : RepairTool(name, 0.65, 250, 2, 0, 9) {}
  };

  class PipeWrench : public RepairTool {
    public:
      explicit PipeWrench(const std::string name) : RepairTool(name, 0.667, 120, 4, 0, 5) {}
  };


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

      int     cur_clipsize;
      double  cur_rof;
      double  cur_reload;
      double  cur_direct_dmg;
      DmgType cur_direct_dmg_type;
      double  cur_aoe_dmg;
      DmgType cur_aoe_dmg_type;
      double  cur_aoe_radius;

      const Ammunition* cur_ammo;

      GunInfo(const GunInfo& obj) : clipsize(obj.clipsize), rof(obj.rof),
          reload_(obj.reload_), direct_dmg(obj.direct_dmg), direct_dmg_type(obj.direct_dmg_type),
          aoe_dmg(obj.aoe_dmg), aoe_dmg_type(obj.aoe_dmg_type), aoe_radius(obj.aoe_radius),
          cur_clipsize(obj.cur_clipsize), cur_rof(obj.cur_rof), cur_reload(obj.cur_reload),
          cur_direct_dmg(obj.cur_direct_dmg), cur_direct_dmg_type(obj.cur_direct_dmg_type),
          cur_aoe_dmg(obj.cur_aoe_dmg), cur_aoe_dmg_type(obj.cur_aoe_dmg_type),
          cur_aoe_radius(obj.cur_aoe_radius), cur_ammo(obj.cur_ammo) {}
      GunInfo& operator=(const GunInfo&) { return *this; };

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
      virtual ~GunInfo() {};

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
  };

  class Gun : public GunInfo, public GoioObj {
    private:
      bool done;

    protected:
      Gun(const std::string name, double max_health,
          int clipsize, double rof, double reload, double direct_dmg,
          DmgType direct_dmg_type, double aoe_dmg, DmgType aoe_dmg_type,
          double aoe_radius) :
                  GunInfo(clipsize, rof, reload, direct_dmg, direct_dmg_type,
                          aoe_dmg, aoe_dmg_type, aoe_radius),
                  GoioObj(name, CmpType::COMPONENTS, 2.33, max_health),
                  done(false) {}

    public:
      virtual ~Gun() {}

      inline double get_rof_changed() const { return get_health()/get_max_health()*get_rof(); }
      inline double get_time_per_shot() const { return get_max_health()/get_health()/get_rof(); }
      inline double get_reload_changed() const { return get_max_health()/get_health()*get_reload(); }

      bool shoot(GoioObj* obj, bool aoe, double aoe_range);
      inline bool shoot(GoioObj* obj) { return shoot(obj, true, 0); }

      TimeFunc get_time_func();
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

  class TimeObj {
    private:
      double time;
      struct FuncData {
        GoioObj* registrar;  // key in registrars
        TimeDmgFunc timedmgfunc;
        GoioObj* obj;
        double time_prev;
        double time_next;    // key in events
        double time_cur;     // if set, not in events (on hold)
        TimeCheckFunc timecheckfunc;
      };

      std::multimap<double, FuncData*> events;
      std::multimap<GoioObj*, FuncData*> registrars;

      bool register_event(FuncData* funcdata, double time);
      bool recalc_next_event(FuncData* funcdata);

    public:
      TimeObj() : time(0), events(), registrars() {}
      ~TimeObj();

      inline double get_time() const { return time; }
      bool next_event();
      bool register_event(GoioObj* registrar, TimeDmgFunc timedmgfunc,
                          GoioObj* obj, TimeCheckFunc timecheckfunc,
                          double time = 0, bool rel = true);
      inline bool done() const { return events.empty(); }

      void reset();
  };

}
