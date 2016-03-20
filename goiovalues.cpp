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

  // while (gat->get_clipsize > 0) {
  // }

  delete gat;
  delete loch;

  return 0;
}
