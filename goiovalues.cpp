#include "goiovalues.h"

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
  auto gat = new goio::Gatling("Gatling1");
  auto loch = new goio::Lochnagar();
  gat->apply_ammunition(loch);

  auto gat2 = new goio::Gatling("Gatling2");

  auto wrench = new goio::PipeWrench("Wrench1");
  auto wrench2 = new goio::PipeWrench("Wrench2");

  auto engi = new goio::MainEngineer("Engi1");
  auto engi2 = new goio::MainEngineer("Engi2");
  auto engi3 = new goio::BuffEngineer("Engi3");

  auto time = new goio::TimeObj();

  auto armor = new goio::Galleon("Gall1");

  using namespace std::placeholders;
  auto gat_id = time->register_event(gat,
                       std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*, double, bool&)>
                                    (&goio::Gatling::shoot), gat, _1, _2, _3),
                       armor,
                       std::bind(&goio::Gatling::get_time_func, gat, _1, _2, _3));
  // auto wrench_id = time->register_event(wrench,
  //                      std::bind(&goio::PipeWrench::repair, wrench, _1, _2, _3),
  //                      armor,
  //                      std::bind(&goio::PipeWrench::get_time_func, wrench, _1, _2, _3),
  //                      1);
  auto wrench2_id = time->register_event(wrench2,
                       std::bind(&goio::PipeWrench::repair, wrench2, _1, _2, _3),
                       gat,
                       std::bind(&goio::PipeWrench::get_time_func, wrench2, _1, _2, _3),
                       1);
  // auto gat2_id = time->register_event(gat2,
  //                      std::bind(static_cast<bool (goio::Gatling::*)(goio::GoioObj*, double, bool&)>
  //                                   (&goio::Gatling::shoot), gat2, _1, _2, _3),
  //                      gat,
  //                      std::bind(&goio::Gatling::get_time_func, gat2, _1, _2, _3),
  //                      30);
  auto engi_id = time->register_event(engi,
                       std::bind(&goio::Engineer::repair, engi, _1, _2, _3),
                       armor,
                       std::bind(&goio::Engineer::get_time_func, engi, _1, _2, _3),
                       1);
  auto engi2_id = time->register_event(engi2,
                       std::bind(&goio::Engineer::repair, engi2, _1, _2, _3),
                       armor,
                       std::bind(&goio::Engineer::get_time_func, engi2, _1, _2, _3),
                       1);
  // auto engi3_id = time->register_event(engi3,
  //                      std::bind(&goio::Engineer::repair, engi3, _1, _2, _3),
  //                      armor,
  //                      std::bind(&goio::Engineer::get_time_func, engi3, _1, _2, _3),
  //                      1);

  std::cout << "\033[1m";
  std::cout << "    time          actor  clip health       target      type  health (R)  type  health (R)" << endl;
  std::cout << "=========================================================================================" << endl;
  std::cout << "\033[0m";
  int i = 0;
  while (time->next_event()) {
    if (++i > 1000) {
      std::cout << "\nevents limit reached!" << std::endl;
      break;
    }
    if (time->get_time() > 500) {
      std::cout << "\ntime limit reached!" << std::endl;
      break;
    }
    // if (time->get_time() > 83)
    //   time->unregister_event(gat2_id);
  }

  time->unregister_event(gat_id);
  // time->unregister_event(wrench_id);
  time->unregister_event(wrench2_id);
  // time->unregister_event(gat2_id);
  time->unregister_event(engi_id);
  time->unregister_event(engi2_id);
  // time->unregister_event(engi3_id);

  delete gat;
  delete gat2;
  delete loch;
  delete armor;
  delete wrench;
  delete wrench2;
  delete engi;
  delete engi2;
  delete engi3;
  delete time;

  return 0;
}
