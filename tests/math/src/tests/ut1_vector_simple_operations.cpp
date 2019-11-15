/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Vector tests
 *
 * @authors    Koszyk
 * created on: 10-11-2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "microhal.h"
#include "microhal_bsp.h"

#include "math/vector.h"

#include "catch.hpp"

using namespace microhal;

SCENARIO("Vector simple operations") {
    GIVEN("A vector with some integer items") {
        Vector<int, 5> vec = {1, 2, 3, 4, 5};

        REQUIRE(1 == vec[0]);
        REQUIRE(2 == vec[1]);
        REQUIRE(3 == vec[2]);
        REQUIRE(4 == vec[3]);
        REQUIRE(5 == vec[4]);

        WHEN("the vector is scalled (float)") {
          float scale = 2.0;
          Vector<int, 5> scalled = vec*scale;
          Vector<int, 5> tester = {2, 4, 6, 8, 10};

          THEN(" the value should match") {
            REQUIRE(scalled == tester);
          }
        }

        WHEN("the vector is scalled (int)") {
          int scale = 2;
          Vector<int, 5> scalled = vec*scale;
          Vector<int, 5> tester = {2, 4, 6, 8, 10};

          THEN(" the value should match") {
            REQUIRE(scalled == tester);
          }
        }

        WHEN("the vector is offset (int)") {
          int offset = 2;
          Vector<int, 5> offseted = vec + offset;
          Vector<int, 5> tester = {3, 4, 5, 6, 7};

          THEN(" the value should match") {
            REQUIRE(offseted == tester);
          }
        }

        WHEN("sum of vectors") {
          Vector<int, 5> ad1 = {1, 2, 3, 4, 5};
          Vector<int, 5> ad2 = {1, 2, 3, 4, 5};
          Vector<int, 5> sum = ad1 + ad2;

          Vector<int, 5> tester = {2, 4, 6, 8, 10};

          THEN(" the value should match") {
            REQUIRE(sum == tester);
          }
        }
        WHEN("diff of vectors") {
          Vector<int, 5> ad1 = {1, 2, 3, 4, 5};
          Vector<int, 5> ad2 = {1, 2, 3, 4, 5};
          Vector<int, 5> diff = ad1 - ad2;

          Vector<int, 5> tester = {0, 0, 0, 0, 0};

          THEN(" the value should match") {
            REQUIRE(diff == tester);
          }
        }

        WHEN("comparisons") {
          Vector<int, 4> vec1 = {1, -1, 1, -1};
          Vector<int, 4> vec2 = {1, -1, 1, 0};

          THEN("unequal") {
            REQUIRE(vec1 != vec2);
          }
          THEN("smaller") {
            REQUIRE(vec1 < vec2);
          }
        }

        WHEN("swaping") {
          Vector<int, 4> vec1 = {1, -1, 1, -1};
          Vector<int, 4> vec2 = {-1, 1, -1, 1};
          Vector<int, 4> swaped(vec2);
          Vector<int, 4> test;

          vec2.swap(vec1);

          REQUIRE(vec1[0] == swaped[0]);
          REQUIRE(vec1[1] == swaped[1]);
          REQUIRE(vec1[2] == swaped[2]);
          REQUIRE(vec1[3] == swaped[3]);

          THEN("equal") {
            REQUIRE(vec1 == swaped);
          }
        }

        WHEN("when iterators works") {
          Vector<int, 5> vec = {1, 2, 3, 4, 5};
          int i;
          int* iterator;
          THEN("you can itarate") {
            for (iterator = vec.begin(), i = 1; iterator != vec.end(); ++iterator, ++i) {
              REQUIRE(*iterator == i);
            }
          }
        }

        WHEN("Specialization allows to") {
            Vector<int, 1> spec1 = {1};
            Vector<int, 1> spec2(spec1 * 2);
            THEN("do that") {
                REQUIRE(spec1[0] == 1);
                REQUIRE(spec2[0] == 2);
            }
        }
    }
}




