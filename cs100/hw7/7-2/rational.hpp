#ifndef RATIONAL
#define RATIONAL
#include <iostream>
// A class support elementary operation for rational number.
// The number are stored and present in fractional form.
// Constructors:
//    Rational(): initialize the number as 0.
//    Rational(int value): initialize the number as `value`
//    Rational(int p, unsigned int q): initialize the number as p/q, note that
//    q cannot be 0.
class Rational {
 public:
  // Constructors
  Rational();
  Rational(int value);
  Rational(int p, unsigned int q);

  // Assignment operator
  Rational& operator=(const Rational& that);

  // Arithmetic operators
  Rational operator+(const Rational& that) const;
  Rational& operator+=(const Rational& that);
  Rational operator-(const Rational& that) const;
  Rational& operator-=(const Rational& that);
  Rational operator*(const Rational& that) const;
  Rational& operator*=(const Rational& that);
  Rational operator/(const Rational& that) const;
  Rational& operator/=(const Rational& that);

  // Comparison operators: equal and less than
  bool operator==(const Rational& that) const;
  bool operator<(const Rational& that) const;
  bool operator>(const Rational& that) const;

  // Output
  friend std::ostream& operator<<(std::ostream& os, const Rational& number);

 private:
  int m_numerator;
  unsigned int m_denominator;
};
#endif
