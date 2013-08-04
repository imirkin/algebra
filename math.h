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

template <typename T>
static T abs(T x) {
  if (x >= 0) return x;
  return -x;
}

template <typename T>
static T min(T x, T y) {
  if (x < y) return x;
  return y;
}

template <typename T>
static T max(T x, T y) {
  if (x > y) return x;
  return y;
}

template <typename T>
static T gcd(T x, T y) {
  x = abs(x);
  y = abs(y);
  T a = min(x, y);
  T b = max(x, y);
  while (a != 0) {
    T nexta = b % a;
    T nextb = a;
    a = nexta;
    b = nextb;
  }
  return b;
}

template <typename T>
static T mod(T a, T b) {
  while (a < 0) a += b;
  return a % b;
}

template <typename T>
static void fixSign(T* vals) {
  if (vals[0] < 0) {
    vals[0] *= -1;
    vals[1] *= -1;
    vals[2] *= -1;
  }
}

/**
 * Returns an array A of length 3. A[0] is the GCD(x,y), and A[0] == A[1] * x
 * + A[2] * y.
 */
template <typename T>
static void extendedGcd(T x, T y, T* ret) {
  if (x > y) {
    extendedGcd(y, x, ret);
    T tmp = ret[1];
    ret[1] = ret[2];
    ret[2] = tmp;
    fixSign(ret);
    return;
  }

  if (x == 0) {
    ret[0] = y;
    ret[1] = 0;
    ret[2] = 1;
    fixSign(ret);
    return;
  } else if (y == 0) {
    ret[0] = x;
    ret[1] = 1;
    ret[2] = 0;
    fixSign(ret);
    return;
  }

  extendedGcd(mod(y, x), x, ret);
  T k = y / x;
  T tmp = ret[2] - k * ret[1];
  ret[2] = ret[1];
  ret[1] = tmp;
  fixSign(ret);
}
