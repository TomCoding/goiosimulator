/**
 * goiovalues - Guns of Icarus Online damage simulator
 * Copyright (C) 2016  Dominique Lasserre
 *
 * This file is part of goiovalues.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "./goiovalues.h"

#include <iostream>
#include <iomanip>


using std::cout;
using std::endl;

int main() {
  goio::init();

  auto mon = new goio::Monitor(1);

  auto gat = new goio::Gatling("Gatling1");
  // auto ammo1 = new goio::Lochnagar();
  auto ammo1 = new goio::Incendiary();
  gat->apply_ammunition(ammo1);
  gat->set_health(10);

  auto gat2 = new goio::Gatling("Gatling2");

  auto banshee = new goio::Banshee("Banshee1");

  auto wrench = new goio::PipeWrench("Wrench1");
  auto wrench2 = new goio::PipeWrench("Wrench2");

  auto ext = new goio::FireExtinguisher("Ext1");
  auto chem = new goio::ChemicalSpray("Chem1");

  auto engi = new goio::MainEngineer("Engi1");
  auto engi2 = new goio::MainEngineer("Engi2");
  auto engi3 = new goio::BuffEngineer("Engi3");

  auto time = new goio::TimeObj();

  auto armor = new goio::Galleon("Gall1");
  armor->set_fire(10);

  auto mon_id = time->register_monitor_event(mon, armor);
  auto gat_id = time->register_shoot_event(gat, armor);
  // auto wrench_id = time->register_repair_event(wrench, armor, 1);
  // auto wrench2_id = time->register_repair_event(wrench2, gat, 1);
  // auto gat2_id = time->register_shoot_event(gat2, gat, 30);
  // auto engi_id = time->register_repair_event(engi, armor, 1);
  // auto engi2_id = time->register_repair_event(engi2, armor, 1);
  // auto engi3_id = time->register_repair_event(engi3, armor, 1);
  // auto ext_id = time->register_repair_event(ext, armor, 1);
  // auto chem_id = time->register_repair_event(chem, armor, 1);
  // auto banshee_id = time->register_shoot_event(banshee, armor);

  std::cout << "\033[1m";
  std::cout << "    time          actor  clip health       target      type  health(R) fire type  health(R)" << endl;
  std::cout << "===========================================================================================" << endl;
  std::cout << "\033[0m";
  int i = 0;
  while (time->next_event()) {
    if (++i > 100000) {
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

  time->unregister_event(mon_id);
  time->unregister_event(gat_id);
  // time->unregister_event(wrench_id);
  // time->unregister_event(wrench2_id);
  // time->unregister_event(gat2_id);
  // time->unregister_event(engi_id);
  // time->unregister_event(engi2_id);
  // time->unregister_event(engi3_id);
  // time->unregister_event(ext_id);
  // time->unregister_event(chem_id);
  // time->unregister_event(banshee_id);

  delete mon;
  delete gat;
  delete gat2;
  delete ammo1;
  delete armor;
  delete wrench;
  delete wrench2;
  delete engi;
  delete engi2;
  delete engi3;
  delete ext;
  delete chem;
  delete banshee;
  delete time;

  return 0;
}
