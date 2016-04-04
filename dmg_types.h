#ifndef __DMG_TYPES_H
#define __DMG_TYPES_H

#include <string>

namespace goio {

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

  /*
   * Radius where aoe damage starts to decrease.
   */
  static constexpr double aoe_radius_dmg_falloff = 0.5;
  /*
   * Minimum damage reduction inside aoe radius.
   */
  static constexpr double aoe_dmg_falloff_min = 0.2;

  /*
   * Ignition chance to apply fire stacks with explosive gun.
   */
  static constexpr double explosive_ign_chance = 0.25;
  /*
   * Applied stacks if explosive ignition chance is met.
   */
  static const int explosive_stacks = 1;

}

#endif // __DMG_TYPES_H
