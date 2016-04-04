#ifndef __GOIOVALUES_H
#define __GOIOVALUES_H

#include "ships.h"
#include "guns.h"
#include "repairtools.h"
#include "ammunitions.h"
#include "timeobj.h"
#include "engineer.h"

#include <stdlib.h>
#include <time.h>

namespace goio {
  inline void init() {
    srand(time(NULL));
  }
}

#endif // __GOIOVALUES_H
