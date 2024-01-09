#ifndef NEWTON_HPP_
#define NEWTON_HPP_

#include <stdlib.h>

#include <cmath>
#include <utility>

template <typename T>
double findNearestRoot(T f, double startingPoint, double eps = 0.000001) {
  auto value = f(startingPoint);
  while (std::fabs(value.first) > eps) {
    startingPoint -= value.first / value.second;
    value = f(startingPoint);
  }
  return startingPoint;
}

#endif  // NEWTON_HPP_