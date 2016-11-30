/*
 * basics.h
 *
 *  Created on: 19 mar 2014
 *      Author: pawel
 */

#ifndef BASICS_H_
#define BASICS_H_

namespace microhal {

static inline uint16_t byteswap(uint16_t data) __attribute__ ((always_inline));
static inline uint32_t byteswap(uint32_t __bsx) __attribute__ ((always_inline));
/* Swap bytes in 16 bit value. */
static inline uint16_t byteswap(uint16_t data) {
    //return ((((__bsx) >> 8) & 0xffu) | (((__bsx) & 0xffu) << 8));
    return (data << 8) | (data >> 8);
}

/* Swap bytes in 32 bit value. */

static inline uint32_t byteswap(uint32_t __bsx) {
    return ((((__bsx) & 0xff000000u) >> 24) | (((__bsx) & 0x00ff0000u) >> 8)
            | (((__bsx) & 0x0000ff00u) << 8) | (((__bsx) & 0x000000ffu) << 24));
}

static inline uint64_t byteswap(uint64_t x) {
  x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
  x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
  x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
  return x;
}

}/* namespace hal*/

#endif /* BASICS_H_ */
