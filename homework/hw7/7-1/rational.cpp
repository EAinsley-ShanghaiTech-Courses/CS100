#include "rational.hpp"

inline int Abs(int a) { return (a > 0 ? a : -a); }

int Gcd(unsigned int a, unsigned int b) {
  while (b) {
    int t = a % b;
    a = b;
    b = t;
  }
  return int(a);
}

// Constructor
Rational::Rational() : m_numerator(0), m_denominator(1){};
Rational::Rational(int value) : m_numerator(value), m_denominator(1){};
Rational::Rational(int p, unsigned int q) {
  if (!q) {
    std::cout << "ERROR: initializing with a denominator of 0!" << std::endl;
    m_numerator = 0, m_denominator = 1;
  } else {
    int common_divisor = Gcd(abs(p), q);
    m_numerator = p / common_divisor;
    m_denominator = q / common_divisor;
  }
}

// copy
Rational& Rational::operator=(const Rational& that) {
  m_numerator = that.m_numerator;
  m_denominator = that.m_denominator;
  return *this;
}

// Arithmetic operators
Rational Rational::operator+(const Rational& that) const {
  unsigned int denominator = m_denominator * that.m_denominator;
  int numerator = m_numerator * int(that.m_denominator) +
                  that.m_numerator * int(m_denominator);
  return Rational(numerator, denominator);
}
Rational& Rational::operator+=(const Rational& that) {
  m_numerator = m_numerator * int(that.m_denominator) +
                that.m_numerator * int(m_denominator);
  m_denominator = m_denominator * that.m_denominator;
  Simplify();
  return *this;
}
Rational Rational::operator-(const Rational& that) const {
  int numerator = m_numerator * int(that.m_denominator) -
                  that.m_numerator * int(m_denominator);
  unsigned int denominator = m_denominator * that.m_denominator;
  return Rational(numerator, denominator);
}
Rational& Rational::operator-=(const Rational& that) {
  m_numerator = m_numerator * int(that.m_denominator) -
                that.m_numerator * int(m_denominator);
  m_denominator = m_denominator * that.m_denominator;
  Simplify();
  return *this;
}
Rational Rational::operator*(const Rational& that) const {
  int numerator = m_numerator * that.m_numerator;
  unsigned int denominator = m_denominator * that.m_denominator;
  return Rational(numerator, denominator);
}
Rational& Rational::operator*=(const Rational& that) {
  m_numerator *= that.m_numerator;
  m_denominator *= that.m_denominator;
  Simplify();
  return *this;
}
Rational Rational::operator/(const Rational& that) const {
  int numerator = m_numerator * int(that.m_denominator);
  if (that.m_numerator < 0) numerator = -numerator;

  unsigned int denominator = m_denominator * abs(that.m_numerator);
  return Rational(numerator, denominator);
}
Rational& Rational::operator/=(const Rational& that) {
  m_numerator *= that.m_denominator;
  if (that.m_numerator < 0) m_numerator = -m_numerator;
  m_denominator *= Abs(that.m_numerator);
  Simplify();
  return *this;
}

// Comparison operators
bool Rational::operator==(const Rational& that) const {
  return (m_numerator == that.m_numerator)
             ? (m_denominator == that.m_denominator)
             : false;
}
bool Rational::operator<(const Rational& that) const {
  return m_numerator * int(that.m_denominator) <
         that.m_numerator * int(m_denominator);
}
// helper function
void Rational::Simplify() {
  int common_divisor = Gcd(abs(m_numerator), m_denominator);
  m_numerator /= common_divisor;
  m_denominator /= common_divisor;
}

// output
std::ostream& operator<<(std::ostream& os, const Rational& number) {
  os << number.m_numerator;
  if (number.m_denominator != 1) os << "/" << number.m_denominator;
  return os;
}
