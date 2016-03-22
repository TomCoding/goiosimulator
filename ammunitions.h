#ifndef __AMMUNITIONS_H
#define __AMMUNITIONS_H

namespace goio {

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

}

#endif // __AMMUNITIONS_H
