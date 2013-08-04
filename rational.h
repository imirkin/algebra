/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Ilia Mirkin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ostream>

#pragma once

template <typename T=int>
class Rational {
 public:
  Rational(T num) : numerator_(num), denominator_(1) {}
  Rational(T num, T denom) : numerator_(num), denominator_(denom) {
    if (denom == 0) throw "div by zero";
    normalize_();
  }
  Rational(const Rational<T>& other) :
      numerator_(other.numerator_), denominator_(other.denominator_) {}
  void operator=(const Rational<T>& other) {
    numerator_ = other.numerator_;
    denominator_ = other.denominator_;
  }
  void operator=(const T& num) {
    numerator_ = num;
    denominator_ = 1;
  }

  bool operator==(const Rational<T>& other) const {
    return numerator_ == other.numerator_ && denominator_ == other.denominator_;
  }

  Rational<T>& operator+=(const Rational<T>& other) {
    numerator_ = numerator_ * other.denominator_ +
      other.numerator_ * denominator_;
    denominator_ *= other.denominator_;
    normalize_();
    return *this;
  }
  Rational<T> operator+(const Rational<T>& other) const {
    return Rational(*this) += other;
  }

  Rational<T>& operator*=(const Rational<T>& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    normalize_();
    return *this;
  }
  Rational<T> operator*(const Rational<T>& other) const {
    return Rational(*this) *= other;
  }

  Rational<T> operator-() {
    return Rational(-numerator_, denominator_);
  }

  void normalize_() {
    T g = gcd(numerator_, denominator_);
    bool negative = (numerator_ >= 0) ^ (denominator_ >= 0);
    if (g == 0) {
      numerator_ = 0;
      denominator_ = 1;
    } else {
      numerator_ = abs(numerator_) / g * (negative ? -1 : 1);
      denominator_ = abs(denominator_) / g;
    }
  }

  T numerator_;
  T denominator_;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Rational<T>& rational) {
  stream << rational.numerator_;
  if (rational.denominator_ != 1)
    stream << "/" << rational.denominator_;
  return stream;
}
