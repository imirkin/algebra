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

#pragma once

#include "math.h"

template <int N, typename T=int>
class IntegerModNOps {
 public:
  IntegerModNOps() {}

  static IntegerModNOps<N, T> instance;

  typedef T element;
  typedef RingElt<IntegerModNOps<N, T> > ring;
  typedef GroupElt<IntegerModNOps<N, T> > group;

  void init(T& a) const {
    a = mod(a, N);
  }

  T zero() const {
    return 0;
  }

  T id() const {
    return 1;
  }

  T negate(const T& a) const {
    return (N - a) % N;
  }

  T plus(const T& a, const T& b) const {
    return (a + b) % N;
  }

  T times(const T& a, const T& b) const {
    return (a * b) % N;
  }

  // This only works for prime N, in groups, when there are no zeroes
  T inv(const T& a) const {
    T extgcd[3] = {0};
    extendedGcd(a, N, extgcd);
    if (extgcd[0] != 1) {
      throw "Attempt to invert non-invertible int";
    }
    return mod(extgcd[1], N);
  }
};
template <int N, typename T>
IntegerModNOps<N, T> IntegerModNOps<N, T>::instance;

template <typename T=int>
class IntegerModOps {
 public:
  IntegerModOps(int N) : N(N) {}

  typedef T element;
  typedef RingElt<IntegerModOps<T> > ring;
  typedef GroupElt<IntegerModOps<T> > group;

  void init(T& a) const {
    a = mod(a, N);
  }

  T zero() const {
    return 0;
  }

  T id() const {
    return 1;
  }

  T negate(const T& a) const {
    return (N - a) % N;
  }

  T plus(const T& a, const T& b) const {
    return (a + b) % N;
  }

  T times(const T& a, const T& b) const {
    return (a * b) % N;
  }

  // This only works for prime N, in groups, when there are no zeroes
  T inv(const T& a) const {
    T extgcd[3] = {0};
    extendedGcd(a, N, extgcd);
    if (extgcd[0] != 1) {
      throw "Attempt to invert non-invertible int";
    }
    return mod(extgcd[1], N);
  }

  int N;
};
