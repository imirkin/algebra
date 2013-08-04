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
    std::unordered_map<typename S::monoid, typename R::ring> terms =
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
