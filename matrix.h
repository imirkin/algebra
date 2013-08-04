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
#include <vector>

#pragma once

template <typename Ops>
class DenseMatrix {
 public:
  typedef typename Ops::ring element;

  DenseMatrix(int width, int height, const element& def) :
      width_(width), height_(height), default_(def) {
    elements_.resize(width * height, def);
  }
  DenseMatrix(const DenseMatrix<Ops>& other) :
      width_(other.width_), height_(other.height_),
      elements_(other.elements_), default_(other.default_) {}
  void operator=(const DenseMatrix<Ops>& other) {
    width_ = other.width_;
    height_ = other.height_;
    elements_ = other.elements_;
    default_ = other.default_;
  }

  // Someone can do matrix[a][b] and have it work as an L-value
  typename std::vector<element>::iterator operator[](int index) {
    return elements_.begin() + index * width_;
  }

  typename std::vector<element>::const_iterator operator[](int index) const {
    return elements_.begin() + index * width_;
  }

  DenseMatrix& operator*=(const DenseMatrix<Ops>& other) {
    *this = *this * other;
    return *this;
  }
  DenseMatrix operator*(const DenseMatrix<Ops>& other) const {
    if (width_ != other.height_) throw "Size mismatch";
    DenseMatrix<Ops> ret(other.width_, height_, default_.zero());
    for (int i = 0; i < other.width_; i++) {
      for (int j = 0; j < height_; j++) {
        element elt = default_.zero();
        for (int n = 0; n < width_; n++) {
          elt = elt + (*this)[j][n] * other[n][i];
        }
        ret[j][i] = elt;
      }
    }
    return ret;
  }

  DenseMatrix& operator+=(const DenseMatrix<Ops>& other) {
    if (width_ != other.width_ || height_ != other.height_) throw "Size mismatch";
    for (int i = 0; i < height_; i++) {
      for (int j = 0; j < width_; j++) {
        (*this)[i][j] += other[i][j];
      }
    }
    return *this;
  }
  DenseMatrix operator+(const DenseMatrix<Ops>& other) {
    return DenseMatrix(*this) += other;
  }

  DenseMatrix& operator*=(const element& other) {
    for (int i = 0; i < height_; i++)
      for (int j = 0; j < width_; j++)
        (*this)[i][j] = (*this)[i][j] * other;
    return *this;
  }

  int width_;
  int height_;
  std::vector<element> elements_;
  element default_;
};

template <typename Ops>
std::ostream& operator<<(std::ostream& stream, const DenseMatrix<Ops>& matrix) {
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
  // TODO: should be possible to make this L-value-capable
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


template <typename Ops>
class DenseMatrixOps {
 public:
  DenseMatrixOps() : elt_ops_(Ops::instance) {}
  DenseMatrixOps(const Ops& ops) : elt_ops_(ops) {}

  static DenseMatrixOps<Ops> instance;

  typedef DenseMatrix<Ops> element;

  void init(element& a) const {
  }

  DenseMatrix<Ops> times(const DenseMatrix<Ops>& a, const DenseMatrix<Ops>& b) const {
    //assert(a.width_ == b.height_);
    return a * b;
  }

  const Ops& elt_ops_;
};

template <int N, typename Ops>
class DenseMatrixNSpace : public DenseMatrixOps<Ops> {
 public:
  DenseMatrixNSpace() : DenseMatrixOps<Ops>() {}
  DenseMatrixNSpace(const Ops& ops) : DenseMatrixOps<Ops>(ops) {}

  static DenseMatrixNSpace<N, Ops> instance;

  typedef DenseMatrix<Ops> element;
  typedef RingElt<DenseMatrixNSpace<N, Ops> > ring;
  typedef GroupElt<DenseMatrixNSpace<N, Ops> > group;

  DenseMatrix<Ops> id() const {
    DenseMatrix<Ops> ret(N, N, this->elt_ops_.zero());
    for (int i = 0; i < N; i++) {
      ret[i][i] = this->elt_ops_.id();
    }
    return ret;
  }

  // TODO: inv

};
template <int N, typename Ops>
DenseMatrixNSpace<N, Ops> DenseMatrixNSpace<N, Ops>::instance;
