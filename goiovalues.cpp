#include "goiovalues.h"

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
  auto gat = new goio::Gatling("Gatling1");

  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);

  auto wrench = new goio::PipeWrench("Wrench1");
  auto wrench2 = new goio::PipeWrench("Wrench2");

  auto engi = new goio::Engineer("Engi1");
  auto engi2 = new goio::Engineer("Engi2");

  auto time = new goio::TimeObj();

  auto armor = new goio::Galleon("Gall1");

  using namespace std::placeholders;
  time->register_event(gat,
                       std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*, bool&)>
                                    (&goio::Gatling::shoot), gat, _1, _2),
                       armor,
                       std::bind(&goio::Gatling::get_time_func, gat, _1, _2, _3));
  time->register_event(wrench,
                       std::bind(&goio::PipeWrench::repair, wrench, _1, _2),
                       armor,
                       std::bind(&goio::PipeWrench::get_time_func, wrench, _1, _2, _3),
                       1);
  time->register_event(wrench2,
                       std::bind(&goio::PipeWrench::repair, wrench2, _1, _2),
                       gat,
                       std::bind(&goio::PipeWrench::get_time_func, wrench2, _1, _2, _3),
                       1);
  // time->register_event(engi,
  //                      std::bind(&goio::Engineer::repair, engi, _1, _2),
  //                      armor,
  //                      std::bind(&goio::Engineer::get_time_func, engi, _1, _2, _3),
  //                      1);
  // time->register_event(engi2,
  //                      std::bind(&goio::Engineer::repair, engi2, _1, _2),
  //                      armor,
  //                      std::bind(&goio::Engineer::get_time_func, engi2, _1, _2, _3),
  //                      1);

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
  delete engi;
  delete engi2;
  delete time;

  return 0;
}
