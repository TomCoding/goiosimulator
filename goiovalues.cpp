#include <iostream>
#include <iomanip>
#include "values.h"
using namespace std;

namespace goio {

}

int main() {
  auto gat = new goio::Gatling();
  cout << gat->get_clipsize() << endl;

  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);
  cout << gat->get_clipsize() << endl;

  auto time = new goio::TimeObj();

  auto armor = new goio::GoioObj("test object", goio::CmpType::ARMOR, 1000, 100);
  time->register_event(std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*)>
                                    (&goio::Gatling::shoot), gat, std::placeholders::_1),
                       armor,
                       std::bind(&goio::Gatling::get_time_per_shot, gat));
  while (time->next_event());

  delete gat;
  delete loch;
  delete armor;
  delete time;

  return 0;
}
