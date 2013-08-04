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

#include <iostream>
#include <utility>

#include "elements.h"
#include "types.h"
#include "spaces.h"

using namespace std;

int main(int argc, char** argv) {

  typedef RingElt<IntegerModNOps<5> > Mod5Ring;
  typedef RingElt<IntegerModOps<> > ModRing;

  IntegerModOps<> mod2 = IntegerModOps<>(2);

  auto id = ModRing(mod2.id(), mod2);

  for (int i = 0; i < 10; i++) {
    std::cout << id << std::endl;
    id = id + 1;
  }

  std::cout << id - 3 << std::endl;
  std::cout << -(id - 3) << std::endl;


  typedef GroupElt<IntegerModNOps<11> > Mod5Group;
  Mod5Group elt = 1;
  for (int i = 0; i < 10; i++, elt = i + 1) {
    std::cout << elt << " * " << 1 / elt << " = 1 (mod11)" << std::endl;
  }

  typedef DenseMatrixNSpace<5, IntegerModNOps<5> > GL5Mod5Space;

  typedef GL5Mod5Space::group GL5Mod5;

  GL5Mod5 mat_id = GL5Mod5Space::instance.id();
  mat_id.element_[1][2] = 4;
  mat_id.element_[2][1] = 3;

  std::cout << mat_id << std::endl;

  mat_id = mat_id * mat_id;

  std::cout << mat_id << std::endl;

  typedef DenseMatrixNSpace<5, IntegerOps<> > GL5Space;
  typedef GL5Space::group GL5;

  GL5 mat = GL5Space::instance.id();
  mat.element_[1][2] = 4;
  mat.element_[2][1] = 3;

  std::cout << mat << std::endl;

  mat = mat * mat;

  std::cout << mat << std::endl;

  // This doesn't work :( There needs to be a conversion between
  // Mod5Ring and IntRing inside of the matrix
  //mat = GL5(mat_id.element_);

  Trace<char> trace {'a', 'b', 'a'}, trace2 {'b', 'a', 'a'};
  std::cout << "trace = " << trace << std::endl;
  std::cout << "trace2 = " << trace2 << std::endl;
  std::cout << "trace == trace2: " << (trace == trace2) << std::endl;

  typedef PolynomialOps<IntegerModNOps<4>, MonomialOps<char> > SemigroupRing1;
  SemigroupRing1 ring1;
  typename SemigroupRing1::element poly;
  poly << make_pair(5, {'a', 'a'});
  poly << make_pair(3, {'a', 'b'});
  poly << make_pair(2, {'b'});

  auto t = SemigroupRing1::ring(poly, ring1);
  std::cout << "(" << t << ") * (" << t << ") = " << t * t << std::endl;
  std::cout << "(" << t << ") + (" << t << ") = " << t + t << std::endl;

  typedef PolynomialOps<IntegerModNOps<4>, MonomialOps<Trace<char> > > SemigroupRing2;
  SemigroupRing2 ring2;
  typename SemigroupRing2::element poly2;
  poly2 << make_pair(5, { Trace<char>({'b', 'a', 'b', 'c'}), Trace<char>({'b', 'c'}) });
  poly2 << make_pair(2, { Trace<char>({'a', 'b', 'c', 'b'}), Trace<char>({'c', 'b'}) });
  poly2 << make_pair(2, { Trace<char>({'a', 'b', 'c', 'b'}), Trace<char>({'c', 'b'}), Trace<char>({'b', 'c'}) });
  std::cout << poly2 << std::endl;

  typedef IntegerOps<Rational<>>::ring Rationals;
  Rationals rat = Rational<>(1, 3);
  rat = rat + Rational<>(1, 6);
  std::cout << rat << std::endl;

  return 0;
}
