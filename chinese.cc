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

// Code largely taken from Josh Grochow's implementation

#include <iostream>
#include <utility>

#include "elements.h"
#include "types.h"
#include "spaces.h"

using namespace std;

int main(int argc, char** argv) {
  typedef IntegerModOps<> Mod;
  typedef typename Mod::ring ring;
  vector<typename Mod::ring> inputs {
    ring(2, Mod(3)),
    ring(3, Mod(4)),
    ring(1, Mod(5))
  };

  // Collect the "B"s from the extended Euclidean algorithm
  int finalMod = 1;
  vector<int> b;
  for (size_t i = 0; i < inputs.size(); i++) {
    finalMod *= inputs[i].ops_.N;
    int curMod = inputs[i].ops_.N;
    int tmpMod = 1;
    for (size_t j = 0; j < inputs.size(); j++) {
      if (i == j) continue;
      tmpMod *= inputs[j].ops_.N;
    }
    int t[3] = {0};
    extendedGcd(curMod, tmpMod, t);
    b.push_back(t[2] * tmpMod);
  }

  auto final = ring(0, Mod(finalMod));
  for (size_t i = 0; i < b.size(); i++) {
    final = final + b[i] * inputs[i];
  }

  std::cout << final << std::endl;
}
