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
#include <functional>

// NOTE: For ops that don't have default constructors, the expectation
// is that they will survive for the duration of the element's
// lifetime.
template <typename Ops>
class SemigroupElt {
  typedef typename Ops::element T;
 public:
  SemigroupElt(const T& element) : element_(element), ops_(Ops::instance) {
    ops_.init(element_);
  }
  SemigroupElt(const T& element, const Ops& ops) : element_(element), ops_(ops) {
    ops_.init(element_);
  }
  SemigroupElt(const SemigroupElt<Ops>& other) : element_(other.element_), ops_(other.ops_) {
    ops_.init(element_);
  }

  void operator=(const SemigroupElt<Ops>& other) {
    element_ = other.element_;
  }
  void operator=(const T& other) {
    element_ = other;
  }

  SemigroupElt operator*(const T& other) const {
    return SemigroupElt(ops_.times(element_, other), ops_);
  }

  bool operator==(const SemigroupElt<Ops>& other) const {
    return element_ == other.element_;
  }
  bool operator!=(const SemigroupElt<Ops>& other) const {
    return !(*this == other);
  }

  operator T&() {
    return element_;
  }

  operator const T&() const {
    return element_;
  }

  T element_;

  const Ops& ops_;
};

template <typename Ops>
std::ostream& operator<<(std::ostream& stream, const SemigroupElt<Ops>& elt) {
  stream << elt.element_;
  return stream;
}

template <typename Ops>
class MonoidElt : public SemigroupElt<Ops> {
  typedef typename Ops::element T;
 public:
  MonoidElt(const T& element) : SemigroupElt<Ops>(element) {}
  MonoidElt(const T& element, const Ops& ops) : SemigroupElt<Ops>(element, ops) {}
  MonoidElt(const MonoidElt<Ops>& other) : SemigroupElt<Ops>(other) {}

  MonoidElt id() const {
    return MonoidElt(this.ops_.id(), this->ops_);
  }

  MonoidElt operator*(const T& other) const {
    return MonoidElt(this->ops_.times(this->element_, other), this->ops_);
  }
};

template <typename Ops>
class GroupElt : public MonoidElt<Ops> {
  typedef typename Ops::element T;
 public:
  GroupElt(const T& element) : MonoidElt<Ops>(element) {}
  GroupElt(const T& element, const Ops& ops) : MonoidElt<Ops>(element, ops) {}
  GroupElt(const GroupElt<Ops>& other) : MonoidElt<Ops>(other) {}

  GroupElt inv() {
    return GroupElt(this->ops_.inv(this->element_), this->ops_);
  }

  GroupElt operator*(const T& other) const {
    return GroupElt(this->ops_.times(this->element_, other), this->ops_);
  }

  GroupElt operator/(const T& other) const {
    return GroupElt(
        this->ops_.times(this->element_, this->ops_.inv(other)),
        this->ops_);
  }

  GroupElt operator^(int n) const {
    T result = this->ops_.id();
    T base = this->element_;
    if (n < 0) {
      base = this->ops_.inv(this->element_);
      n = -n;
    }
    while (n > 0) {
      if (n % 2 == 1) {
        result = this->ops_.times(result, base);
        n--;
      }
      base = this->ops_.times(base, base);
      n /= 2;
    }
    return GroupElt(result, this->ops_);
  }
};

template <typename Ops>
GroupElt<Ops> operator/(const typename Ops::element& a, const GroupElt<Ops>& b) {
  return GroupElt<Ops>(b.ops_.times(a, b.ops_.inv(b)), b.ops_);
}


template <typename Ops>
class RingElt : public MonoidElt<Ops> {
  typedef typename Ops::element T;
 public:
  RingElt(const T& element) : MonoidElt<Ops>(element) {}
  RingElt(const T& element, const Ops& ops) : MonoidElt<Ops>(element, ops) {}
  RingElt(const RingElt<Ops>& other) : MonoidElt<Ops>(other) {}

  RingElt id() const {
    return RingElt(this->ops_.id(), this->ops_);
  }
  RingElt zero() const {
    return RingElt(this->ops_.zero(), this->ops_);
  }

  RingElt operator-() const {
    return RingElt(this->ops_.negate(this->element_), this->ops_);
  }
  RingElt operator-(const T& other) const {
    return RingElt(this->ops_.plus(this->element_, this->ops_.negate(other)), this->ops_);
  }
  RingElt operator+(const T& other) const {
    return RingElt(this->ops_.plus(this->element_, other), this->ops_);
  }
  RingElt operator*(const T& other) const {
    return RingElt(this->ops_.times(this->element_, other), this->ops_);
  }
};
