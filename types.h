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

#include <unordered_map>
#include <initializer_list>
#include <vector>
#include <algorithm>

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

template <typename T=int>
class Rational {
 public:
  Rational(T num) : numerator_(num), denominator_(1) {}
  Rational(T num, T denom) : numerator_(num), denominator_(denom) {
    if (denom == 0) throw "div by zero";
    normalize_();
  }
  Rational(const Rational<T>& other) :
      numerator_(other.numerator_), denominator_(other.denominator_) {
    normalize_();
  }
  void operator=(const Rational<T>& other) {
    numerator_ = other.numerator_;
    denominator_ = other.denominator_;
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
  stream << rational.numerator_ << "/" << rational.denominator_;
  return stream;
}



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

template <typename R, typename S>
class Polynomial {
 public:
  Polynomial() {}
  Polynomial(const Polynomial<R, S>& other) : components_(other.components_) {}
  void operator=(const Polynomial<R, S>& other) {
    components_ = other.components_;
  }

  Polynomial& operator<<(
      const std::pair<typename R::ring, typename S::monoid>& term) {
    if (term.first == term.first.zero()) {
      return *this;
    }
    auto it = components_.find(term.second);
    if (it == components_.end()) {
      components_.insert(make_pair(term.second, term.first));
    } else {
      it->second = it->second + term.first;
      if (it->second == it->second.zero()) {
        components_.erase(it);
      }
    }
    return *this;
  }

  std::unordered_map<typename S::monoid, typename R::ring> components_;
};

template <typename R, typename S>
std::ostream& operator<<(std::ostream& stream, const Polynomial<R, S>& poly) {
  for (auto it = poly.components_.begin(); it != poly.components_.end(); ++it) {
    if (it != poly.components_.begin()) {
      stream << " + ";
    }
    if (it->second != it->second.id()) {
      stream << it->second;
    }
    stream << it->first;
  }
  return stream;
}

template <typename E>
class DenseMatrix {
 public:
  typedef typename std::vector<E>::iterator iterator;

  DenseMatrix(int width, int height, const E& def) : width_(width), height_(height) {
    elements_.resize(width * height, def);
  }
  DenseMatrix(const DenseMatrix<E>& other) :
      width_(other.width_), height_(other.height_), elements_(other.elements_) {}
  void operator=(const DenseMatrix<E>& other) {
    width_ = other.width_;
    height_ = other.height_;
    elements_ = other.elements_;
  }

  // Someone can do matrix[a][b] and have it work as an L-value
  typename std::vector<E>::iterator operator[](int index) {
    return elements_.begin() + index * width_;
  }

  typename std::vector<E>::const_iterator operator[](int index) const {
    return elements_.begin() + index * width_;
  }

  int width_;
  int height_;
  std::vector<E> elements_;
};

template <typename E>
std::ostream& operator<<(std::ostream& stream, const DenseMatrix<E>& matrix) {
  for (int i = 0; i < matrix.height_; i++) {
    for (int j = 0; j < matrix.width_; j++) {
      stream << " " << matrix[i][j];
    }
    stream << std::endl;
  }
  return stream;
}

template <typename E>
class SparseMatrix {
 public:
  SparseMatrix(int width, int height, const E& def) :
      width_(width), height_(height), default_(def) {}
  SparseMatrix(const SparseMatrix<E>& other) :
      width_(other.width_), height_(other.height_), default_(other.default_),
      elements_(other.elements_) {}
  void operator=(const SparseMatrix<E>& other) {
    width_ = other.width_;
    height_ = other.height_;
    default_ = other.default_;
    elements_ = other.elements_;
  }

  struct iterator {
    iterator(int index) : index1(index) {}
    int index1;

    const E& operator[](int index2) {
      auto it = elements_.find(index1 * width_ + index2);
      if (it == elements_.end()) {
        return default_;
      }
      return *it;
    }
  };

  // Someone can do matrix[a][b], but only as an R-value
  iterator operator[](int index) const {
    return iterator(index);
  }

  E& set(int row, int col, const E& value) {
    return elements_[row * width_ + col] = value;
  }

  int width_;
  int height_;
  E default_;
  std::unordered_map<int, E> elements_;
};




// NOTE: These have to be here because they reference the other hash
// functions. Also, for reasons not-understood, it didn't seem to want
// to infer types correctly.
namespace std {
  template <typename T>
  struct hash<SemigroupElt<T> > {
    size_t operator()(const SemigroupElt<T>& e) const {
      return hash<typename T::element>()(e.element_);
    }
  };

  template <typename T>
  struct hash<MonoidElt<T> > {
    size_t operator()(const MonoidElt<T>& e) const {
      return hash<typename T::element>()(e.element_);
    }
  };

  template <typename T>
  struct hash<GroupElt<T> > {
    size_t operator()(const GroupElt<T>& e) const {
      return hash<typename T::element>()(e.element_);
    }
  };

  template <typename T>
  struct hash<RingElt<T> > {
    size_t operator()(const RingElt<T>& e) const {
      return hash<typename T::element>()(e.element_);
    }
  };
}
