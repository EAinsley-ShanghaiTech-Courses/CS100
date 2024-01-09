#ifndef FUNCTIONAL_FORMS_HPP_
#define FUNCTIONAL_FORMS_HPP_

#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <utility>

auto derSum = [](auto f1, auto f2) {
  return [=](auto m) {
    auto pairf1 = f1(m), pairf2 = f2(m);
    return std::make_pair(pairf1.first + pairf2.first,
                          pairf1.second + pairf2.second);
  };
};

auto derSub = [](auto f1, auto f2) {
  return [=](auto m) {
    auto pairf1 = f1(m), pairf2 = f2(m);
    return std::make_pair(pairf1.first - pairf2.first,
                          pairf1.second - pairf2.second);
  };
};

auto derMul = [](auto f1, auto f2) {
  return [=](auto m) {
    auto pairf1 = f1(m), pairf2 = f2(m);
    return std::make_pair(
        pairf1.first * pairf2.first,
        pairf1.first * pairf2.second + pairf1.second * pairf2.first);
  };
};

auto derDiv = [](auto f1, auto f2) {
  return [=](auto m) {
    auto pairf1 = f1(m), pairf2 = f2(m);
    return std::make_pair(
        double(pairf1.first) / double(pairf2.first),
        double(pairf1.second * pairf2.first - pairf1.first * pairf2.second) /
            double(pairf2.first * pairf2.first));
  };
};

auto derComp = [](auto f1, auto f2) {
  return [=](auto m) {
    auto pairf1 = f1(m), pairf2of1 = f2(pairf1.first);
    return std::make_pair(pairf2of1.first, pairf2of1.second * pairf1.second);
  };
};

auto derPow = [](auto f, int exp) {
  return [=](auto m) {
    auto pairf = f(m);
    double expf = pow(pairf.first, exp - 1);
    return std::make_pair(expf * pairf.first, exp * expf * pairf.second);
  };
};

#endif  // FUNCTIONAL_FORMS_HPP_