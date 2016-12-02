/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Vector
 *
 * @authors    Pawel
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

#ifndef _MICROHAL_VECTOR_H_
#define _MICROHAL_VECTOR_H_

#include <stdint.h>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <initializer_list>

namespace microhal {
template<typename T, std::size_t N>
class Vector : public std::array<T, N> {
    static_assert(N > 0, "Vector size must be positive");

 public:
    Vector() {
        std::size_t elementIndex = 0;
        for (elementIndex = 0; elementIndex < N; ++elementIndex) {
          this->at(elementIndex) = T();
        }
    }

    Vector(std::initializer_list<T> il) {
      std::size_t elementIndex = 0;
      for (const T* it=std::begin(il); it != std::end(il) && elementIndex < N; ++it, ++elementIndex) {
        this->at(elementIndex) = *it;
      }
    }
    Vector(const Vector& vec) {
      std::copy(vec.begin(), vec.end(), this->begin());
    }

    Vector& operator= (const Vector & vec) {
      std::copy(vec.begin(), vec.end(), this->begin());
      return *this;
    }

    // vector specific operators
    Vector<T, N> operator*(const float scale) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result.at(index) = this->at(index) * scale;
       }
       return result;
     }

    Vector<T, N> operator*(const int32_t scale) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result[index] = this->at(index) * scale;
       }
       return result;
     }

    Vector<T, N> operator+(const float offset) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result[index]  = this->at(index) + offset;
       }
       return result;
     }

    Vector<T, N> operator+(const int32_t offset) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result[index] = this->at(index) + offset;
       }
       return result;
     }

    Vector<T, N> operator+(const Vector<T, N>& second) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result[index] = this->at(index) + second[index];
       }
       return result;
     }

    Vector<T, N> operator-(const Vector<T, N>& second) const {
        Vector<T, N> result(*this);
       for (size_t index = 0; index < N; ++index) {
         result[index] = this->at(index) - second[index];
       }
       return result;
     }
};

template<typename T>
class Vector<T, 1> : public std::array<T, 1> {
 public:
    Vector() {
       this->at(0) = T();
    }

    Vector(std::initializer_list<T> il) {
        this->at(0) = *(il.begin());
    }
    Vector(const Vector& vec) {
        this->at(0) = *(vec.begin());
    }

    Vector& operator= (const Vector & vec) {
        this->at(0) = *(vec.begin());
      return *this;
    }

    // vector specific operators
    Vector<T, 1> operator*(const float scale) const {
        Vector<T, 1> result = {this->at(0) * scale};
        return result;
     }

    Vector<T, 1> operator*(const int32_t scale) const {
        Vector<T, 1> result = {this->at(0) * scale};
        return result;
     }

    Vector<T, 1> operator+(const float offset) const {
        Vector<T, 1> result = {this->at(0) + offset};
        return result;
     }

    Vector<T, 1> operator+(const int32_t offset) const {
        Vector<T, 1> result = {this->at(0) + offset};
        return result;
     }

    Vector<T, 1> operator+(const Vector<T, 1>& second) const {
        Vector<T, 1> result = {this->at(0) + second.at(0)};
        return result;
     }

    Vector<T, 1> operator-(const Vector<T, 1>& second) const {
        Vector<T, 1> result = {this->at(0) - second.at(0)};
        return result;
     }
};
}  // namespace microhal

#endif  // _MICROHAL_VECTOR_H_
