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

#include <tr1/unordered_map>
#include <vector>

// TODO: Rewrite all the ops to mutate a so that we can write more
// efficient += operators.

template <typename Ops>
class DenseMatrixOps {
 public:
  DenseMatrixOps() : elt_ops_(Ops::instance) {}
  DenseMatrixOps(const Ops& ops) : elt_ops_(ops) {}

  static DenseMatrixOps<Ops> instance;

  typedef RingElt<Ops> R;
  typedef DenseMatrix<R> element;

  void init(element& a) const {
  }

  DenseMatrix<R> times(const DenseMatrix<R>& a, const DenseMatrix<R>& b) const {
    //assert(a.width_ == b.height_);
    if (a.width_ != b.height_) throw "Size mismatch";
    DenseMatrix<R> ret(b.width_, a.height_, elt_ops_.zero());
    for (int i = 0; i < b.width_; i++) {
      for (int j = 0; j < a.height_; j++) {
        R elt = elt_ops_.zero();
        for (int n = 0; n < a.width_; n++) {
          elt = elt + a[j][n] * b[n][i];
        }
        ret[j][i] = elt;
      }
    }
    return ret;
  }

  const Ops& elt_ops_;
};

template <int N, typename Ops>
class DenseMatrixNSpace : public DenseMatrixOps<Ops> {
 public:
  DenseMatrixNSpace() : DenseMatrixOps<Ops>() {}
  DenseMatrixNSpace(const Ops& ops) : DenseMatrixOps<Ops>(ops) {}

  static DenseMatrixNSpace<N, Ops> instance;

  typedef RingElt<Ops> R;
  typedef DenseMatrix<R> element;
  typedef RingElt<DenseMatrixNSpace<N, Ops> > ring;
  typedef GroupElt<DenseMatrixNSpace<N, Ops> > group;

  DenseMatrix<R> id() const {
    DenseMatrix<R> ret(N, N, this->elt_ops_.zero());
    for (int i = 0; i < N; i++) {
      ret[i][i] = this->elt_ops_.id();
    }
    return ret;
  }

  // TODO: inv

};
template <int N, typename Ops>
DenseMatrixNSpace<N, Ops> DenseMatrixNSpace<N, Ops>::instance;

template <typename T=int>
class IntegerOps {
 public:
  static IntegerOps<T> instance;

  typedef T element;
  typedef RingElt<IntegerOps<T> > ring;

  void init(T& a) const {
  }

  T zero() const {
    return 0;
  }

  T id() const {
    return 1;
  }

  T negate(const T& a) const {
    return -a;
  }

  T plus(const T& a, const T& b) const {
    return a + b;
  }

  T times(const T& a, const T& b) const {
    return a * b;
  }
};
template <typename T>
IntegerOps<T> IntegerOps<T>::instance;


// NOTE: Assumes the pre-existing values are all non-negative and < N.
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
    element ret = a;
    for (auto it = b.exponents_.begin(); it != b.exponents_.end(); ++it) {
      ret << *it;
    }
    return ret;
  }
};
template <typename T>
MonomialOps<T> MonomialOps<T>::instance;

template <typename R, typename S>
class PolynomialOps {
 public:
  PolynomialOps() : ring_ops_(R::instance), semigroup_ops_(S::instance) {}
  PolynomialOps(const R& ring_ops, const S& semigroup_ops) :
      ring_ops_(ring_ops), semigroup_ops_(semigroup_ops) {}

  typedef Polynomial<R, S> element;
  typedef RingElt<PolynomialOps<R, S> > ring;

  void init(element& a) const {
  }

  element zero() const {
    return element();
  }

  element id() const {
    return element() << make_pair(ring_ops_.id(), semigroup_ops_.id());
  }

  element plus(const element& a, const element& b) const {
    // Need to group identical semigroup elements and add up their coefficients.
    std::tr1::unordered_map<typename S::monoid, typename R::ring> terms =
      a.components_;
    for (auto it = b.components_.begin(); it != b.components_.end(); ++it) {
      auto existing_it = terms.find(it->first);
      if (existing_it == terms.end()) {
        terms.insert(*it);
      } else {
        existing_it->second = existing_it->second + it->second;
        if (existing_it->second == existing_it->second.zero()) {
          terms.erase(existing_it);
        }
      }
    }
    element ret;
    ret.components_ = terms;
    return ret;
  }

  element times(const element& a, const element& b) const {
    // zip([a*b for a in r1 for b in r2],
    //     ["".join(sorted(a+b)) for a in s1 for b in s2])
    //
    // And then group ring values based on the semigroup elements
    std::vector<typename S::monoid> s;
    std::vector<typename R::ring> r;

    size_t total = a.components_.size() * b.components_.size();
    s.reserve(total);
    r.reserve(total);

    for (auto a_it = a.components_.begin(); a_it != a.components_.end(); ++a_it) {
      for (auto b_it = b.components_.begin(); b_it != b.components_.end(); ++b_it) {
        s.push_back(a_it->first * b_it->first);
        r.push_back(a_it->second * b_it->second);
      }
    }
    element ret;
    for (size_t i = 0; i < s.size(); i++) {
      auto s_i = s[i];
      auto r_i = r[i];
      if (r_i == r_i.zero()) {
        continue;
      }
      auto it = ret.components_.find(s_i);
      if (it == ret.components_.end()) {
        ret.components_.insert(make_pair(s_i, r_i));
      } else {
        it->second = it->second + r_i;
        if (it->second == it->second.zero()) {
          ret.components_.erase(it);
        }
      }
    }
    return ret;
  }

  const R& ring_ops_;
  const S& semigroup_ops_;
};
