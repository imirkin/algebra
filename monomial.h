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
#include <unordered_map>
#include <initializer_list>

#pragma once

template <typename T>
class Monomial {
 public:
  Monomial() {}
  Monomial(const Monomial<T>& other) : exponents_(other.exponents_) {}
  Monomial(std::initializer_list<T> init) {
    for (auto it = init.begin(); it != init.end(); ++it) {
      *this << *it;
    }
  }
  void operator=(const Monomial<T>& other) {
    exponents_ = other.exponents_;
  }

  bool operator==(const Monomial<T>& other) const {
    if (exponents_.size() != other.exponents_.size()) {
      return false;
    }
    for (auto it = exponents_.begin(); it != exponents_.end(); ++it) {
      auto other_it = other.exponents_.find(it->first);
      if (other_it == other.exponents_.end())
        return false;
      if (it->second != other_it->second)
        return false;
    }
    return true;
  }
  bool operator!=(const Monomial<T>& other) {
    return !(*this == other);
  }

  Monomial& operator*=(const Monomial<T>& other) {
    for (auto it = other.exponents_.begin(); it != other.exponents_.end(); ++it) {
      *this << *it;
    }
    return *this;
  }
  Monomial operator*(const Monomial<T>& other) const {
    return Monomial(*this) *= other;
  }

  Monomial& operator<<(const T& term) {
    auto it = exponents_.find(term);
    if (it != exponents_.end()) {
      it->second++;
    } else {
      exponents_.insert(make_pair(term, 1));
    }
    return *this;
  }

  Monomial& operator<<(const std::pair<T, int>& pair) {
    auto it = exponents_.find(pair.first);
    if (it != exponents_.end()) {
      it->second += pair.second;
    } else {
      exponents_.insert(pair);
    }
    return *this;
  }

  std::unordered_map<T, int> exponents_;
};
namespace std {
  template <typename T>
  struct hash<Monomial<T> > {
    size_t operator()(const Monomial<T>& monomial) const {
      // TODO: For this to be consistent, need to sort the elements first...
      size_t ret = 0;
      for (auto it = monomial.exponents_.begin(); it != monomial.exponents_.end(); ++it) {
        ret *= 31;
        ret += hash<T>()(it->first);
        ret *= 31;
        ret += hash<int>()(it->second);
      }
      return ret;
    }
  };
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Monomial<T>& monomial) {
  for (auto it = monomial.exponents_.begin(); it != monomial.exponents_.end(); ++it) {
    stream << it->first;
    if (it->second > 1) {
      stream << "^" << it->second;
    }
  }
  return stream;
}

template <typename T>
class MonomialOps {
 public:
  MonomialOps() {}

  static MonomialOps<T> instance;

  typedef Monomial<T> element;
  typedef SemigroupElt<MonomialOps<T> > semigroup;
  typedef MonoidElt<MonomialOps<T> > monoid;

  void init(element& a) const {
  }

  element id() const {
    return element();
  }

  element times(const element& a, const element& b) const {
    return a * b;
  }
};
template <typename T>
MonomialOps<T> MonomialOps<T>::instance;
