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
#include <vector>
#include <initializer_list>

#pragma once

template <typename T>
class Word {
 public:
  Word() {}
  Word(const Word<T>& other) : elements_(other.elements_) {}
  Word(std::initializer_list<T> init) {
    elements_.reserve(init.size());
    std::copy(init.begin(), init.end(), std::back_inserter(elements_));
  }
  void operator=(const Word<T>& other) {
    elements_ = other.elements_;
  }

  bool operator==(const Word<T>& other) const {
    if (elements_.size() != other.elements_.size()) {
      return false;
    }
    return std::equal(elements_.begin(), elements_.end(),
                      other.elements_.begin());
  }
  bool operator!=(const Word<T>& other) const {
    return !(*this == other);
  }

  Word& operator<<(const T& t) {
    elements_.push_back(t);
    return *this;
  }
 public:
  std::vector<T> elements_;
};
namespace std {
  template <typename T>
  struct hash<Word<T> > {
    size_t operator()(const Word<T>& word) const {
      size_t ret = 0;
      for (auto it = word.elements_.begin(); it != word.elements_.end(); ++it) {
        ret *= 31;
        ret += hash<T>()(*it);
      }
      return ret;
    }
  };
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Word<T>& word) {
  for (auto it = word.elements_.begin(); it != word.elements_.end(); ++it) {
    stream << *it;
  }
  return stream;
}
