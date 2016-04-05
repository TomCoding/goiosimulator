#ifndef RNG_H_
#define RNG_H_

#include <random>
#include <functional>


namespace goio {

extern std::function<double ()> percentage;

inline void init() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> uni(0,100);

  percentage = std::bind(uni, rng);
}

}  // namespace goio


#endif  // RNG_H_
