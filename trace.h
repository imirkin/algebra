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
#include <initializer_list>
#include <algorithm>

#include "word.h"

#pragma once

template <typename T>
class Trace {
 public:
  Trace() {}
  Trace(const Trace& other) : word_(other.word_) {}
  Trace(std::initializer_list<T> init) : word_(init) {}
  void operator=(const Trace& other) {
    word_ = other.word_;
  }

  bool operator==(const Trace& other) const {
    size_t n = word_.elements_.size();
    if (n != other.word_.elements_.size()) {
      return false;
    }
    // Cyclic comparison.
    for (size_t i = 0; i < n; i++) {
      size_t j;
      for (j = 0; j < n; j++) {
        if (word_.elements_[j] != other.word_.elements_[(i + j) % n])
          break;
      }
      if (j == n) {
        return true;
      }
    }
    return false;
  }
  bool operator!=(const Trace& other) {
    return !(*this == other);
  }

  Trace& operator<<(const T& c) {
    word_ << c;
    return *this;
  }

  Word<T> word_;
};
namespace std {
  template <typename T>
  struct hash<Trace<T> > {
    size_t operator()(const Trace<T>& trace) const {
      std::vector<T> elements = trace.word_.elements_;
      std::sort(elements.begin(), elements.end());
      size_t ret = 0;
      for (size_t i = 0; i < elements.size(); i++) {
        ret += hash<T>()(elements[i]);
        ret *= 31;
      }
      return ret;
    }
  };
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Trace<T>& trace) {
  stream << "tr(" << trace.word_ << ")";
  return stream;
}
