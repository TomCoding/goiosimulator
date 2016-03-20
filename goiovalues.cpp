#include <iostream>
#include "values.h"
using namespace std;

namespace goio {

}

int main(int argc, const char *argv[]) {
  auto gat = new goio::Gatling();
  cout << gat->get_clipsize() << endl;

  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);
  cout << gat->get_clipsize() << endl;

  auto armor = new goio::GoioObj(goio::CmpType::ARMOR, 1000, 100);
  while (gat->get_clipsize() > 0 && gat->get_health() > 0) {
    gat->shoot(armor);
  }

  delete gat;
  delete loch;
  delete armor;

  return 0;
}
