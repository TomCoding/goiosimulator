#include "values.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace goio {

}

int main() {
  auto gat = new goio::Gatling("Gatling1");

  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);

  auto wrench = new goio::PipeWrench("Wrench1");
  auto wrench2 = new goio::PipeWrench("Wrench2");

  auto time = new goio::TimeObj();

  auto armor = new goio::Galleon("Gall1");
  time->register_event(gat,
                       std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*)>
                                    (&goio::Gatling::shoot), gat, std::placeholders::_1),
                       armor,
                       std::bind(&goio::Gatling::get_time_func, gat));
  time->register_event(wrench,
                       std::bind(&goio::PipeWrench::repair, wrench, std::placeholders::_1),
                       armor,
                       std::bind(&goio::PipeWrench::get_time_func, wrench),
                       1);
  time->register_event(wrench2,
                       std::bind(&goio::PipeWrench::repair, wrench2, std::placeholders::_1),
                       gat,
                       std::bind(&goio::PipeWrench::get_time_func, wrench2),
                       1);
  std::cout << "\033[1m";
  std::cout << "    time          actor  clip health       target      type  health (R)  type  health (R)" << endl;
  std::cout << "=========================================================================================" << endl;
  std::cout << "\033[0m";
  int i = 0;
  while (time->next_event() || !time->done())
    if (++i > 1000 || time->get_time() > 900) break;

  delete gat;
  delete loch;
  delete armor;
  delete wrench;
  delete wrench2;
  delete time;

  return 0;
}
