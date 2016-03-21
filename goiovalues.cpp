// #include <iostream>
// #include <iomanip>
#include "values.h"
using namespace std;

namespace goio {

}

int main() {
  auto gat = new goio::Gatling("Gatling1");
  // cout << gat->get_clipsize() << endl;

  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);
  // cout << gat->get_clipsize() << endl;

  auto time = new goio::TimeObj();

  auto armor = new goio::Galleon("Gall1");
  time->register_event(gat,
                       std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*)>
                                    (&goio::Gatling::shoot), gat, std::placeholders::_1),
                       armor,
                       std::bind(&goio::Gatling::get_time_func, gat));
  int i = 0;
  while (time->next_event())
    if (++i > 60) break;

  delete gat;
  delete loch;
  delete armor;
  delete time;

  return 0;
}
