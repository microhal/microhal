/*
 * proccessor.h
 *
 *  Created on: 1 kwi 2014
 *      Author: pawel
 */

#ifndef PROCCESSOR_H_
#define PROCCESSOR_H_

namespace microhal {
namespace stm32f4xx {

typedef struct {
    const char *name;
    ByteOrder byteOrder;

} ProcessorInfo;

constexpr ProcesorInfo processorInfo = {.name = "stm32f4xx",
    .byteOrder = LSB_FIRST};


}
}

#endif /* PROCCESSOR_H_ */
