#ifndef POLY_HPP_
#define POLY_HPP_
#include <stdlib.h>

#include <vector>
template <typename T>
class Poly {
 public:
  Poly() = default;
  Poly(T constant) : coeffs_(1, constant){};
  Poly(const std::vector<T>& coeffs) : coeffs_(coeffs){};
  Poly(std::vector<T>&& coeffs) : coeffs_(coeffs){};
  Poly(T* coeffs, size_t numberCoeffs)
      : coeffs_(coeffs, coeffs + numberCoeffs){};
  Poly(std::initializer_list<T> coeffs) : coeffs_(coeffs){};
  virtual ~Poly(){};
  Poly(const Poly<T>& poly) = default;
  Poly(Poly<T>&& poly) = default;
  Poly<T>& operator=(const Poly<T>& poly) {
    coeffs_ = poly.coeffs_;
    return *this;
  };
  Poly<T>& operator=(Poly<T>&& poly) {
    coeffs_ = poly.coeffs_;
    return *this;
  };
  inline Poly<T> operator+(const Poly<T>& rhs) const;
  inline Poly<T> operator-(const Poly<T>& rhs) const;
  inline Poly<T> operator*(const Poly<T>& rhs) const;
  inline Poly<T>& operator+=(const Poly<T>& rhs);
  inline Poly<T>& operator-=(const Poly<T>& rhs);
  Poly<T>& operator*=(const Poly<T>& rhs);
  T eval(T param) const;
  Poly<T> der() const;
  std::pair<T, T> operator()(T param) const;

 private:
  std::vector<T> coeffs_;
};
template <typename T>
Poly<T> Poly<T>::operator+(const Poly<T>& rhs) const {
  Poly<T> new_poly(*this);
  return new_poly += rhs;
}
template <typename T>
Poly<T> Poly<T>::operator-(const Poly<T>& rhs) const {
  Poly<T> new_poly(*this);
  return new_poly -= rhs;
}
template <typename T>
Poly<T> Poly<T>::operator*(const Poly<T>& rhs) const {
  Poly new_poly(*this);
  return new_poly *= rhs;
}
template <typename T>
Poly<T>& Poly<T>::operator+=(const Poly<T>& rhs) {
  if (coeffs_.size() < rhs.coeffs_.size()) {
    for (size_t i = 0; i < coeffs_.size(); ++i) coeffs_[i] += rhs.coeffs_[i];
    for (size_t i = coeffs_.size(); i < rhs.coeffs_.size(); ++i)
      coeffs_.push_back(rhs.coeffs_[i]);
  } else {
    for (size_t i = 0; i < rhs.coeffs_.size(); ++i)
      coeffs_[i] += rhs.coeffs_[i];
  }
  return *this;
}
template <typename T>
Poly<T>& Poly<T>::operator-=(const Poly<T>& rhs) {
  if (coeffs_.size() < rhs.coeffs_.size()) {
    for (size_t i = 0; i < coeffs_.size(); ++i) coeffs_[i] -= rhs.coeffs_[i];
    for (size_t i = coeffs_.size(); i < rhs.coeffs_.size(); ++i)
      coeffs_.push_back(-rhs.coeffs_[i]);
  } else {
    for (size_t i = 0; i < rhs.coeffs_.size(); ++i)
      coeffs_[i] -= rhs.coeffs_[i];
  }
  return *this;
}
template <typename T>
Poly<T>& Poly<T>::operator*=(const Poly<T>& rhs) {
  std::vector<T> result_coeffs(coeffs_.size() + rhs.coeffs_.size() - 1);
  for (size_t i = 0; i < coeffs_.size(); ++i)
    for (size_t j = 0; j < rhs.coeffs_.size(); ++j) {
      result_coeffs[i + j] += coeffs_[i] * rhs.coeffs_[j];
    }
  coeffs_ = result_coeffs;
  return *this;
}
template <typename T>
T Poly<T>::eval(T param) const {
  T result_number = T(), multiple_number = T(1);
  for (const auto coeff : coeffs_) {
    result_number += multiple_number * coeff;
    multiple_number *= param;
  }
  return result_number;
}
template <typename T>
Poly<T> Poly<T>::der() const {
  Poly<T> derivative(std::vector<T>(coeffs_.size() - 1));
  for (size_t i = 0; i < coeffs_.size() - 1; ++i)
    derivative.coeffs_[i] = (i + 1) * coeffs_[i + 1];
  return derivative;
}
template <typename T>
std::pair<T, T> Poly<T>::operator()(T param) const {
  return std::pair<T, T>(eval(param), der().eval(param));
}

#endif  // POLY_HPP