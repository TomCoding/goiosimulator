#ifndef __SHIPS_H
#define __SHIPS_H

#include "goioobj.h"

namespace goio {

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

}

#endif // __SHIPS_H
