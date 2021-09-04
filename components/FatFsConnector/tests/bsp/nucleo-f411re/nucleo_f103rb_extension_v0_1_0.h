#ifndef NUCLEO_F411RE_EXTENSION_V0_1_0_H_
#define NUCLEO_F411RE_EXTENSION_V0_1_0_H_

/* *********************************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"

namespace bsp {    
namespace con1 {
namespace a {
    constexpr microhal::IOPin io1{microhal::IOPin::PortC, 11};
    constexpr microhal::IOPin io2{microhal::IOPin::PortC, 10};
    constexpr microhal::IOPin io3{microhal::IOPin::PortD, 2};
    constexpr microhal::IOPin io4{microhal::IOPin::PortB, 13};
    constexpr microhal::IOPin io5{microhal::IOPin::PortC, 0};
    constexpr microhal::IOPin io6{microhal::IOPin::PortC, 12};
    constexpr microhal::IOPin sda{microhal::IOPin::PortB, 9};
    constexpr microhal::IOPin scl{microhal::IOPin::PortB, 8};
    constexpr microhal::IOPin mosi{microhal::IOPin::PortA, 7};
    constexpr microhal::IOPin miso{microhal::IOPin::PortA, 6};
    constexpr microhal::IOPin sck{microhal::IOPin::PortA, 5};
    constexpr microhal::IOPin cs{microhal::IOPin::PortA, 15};
    constexpr microhal::IOPin rxd{microhal::IOPin::PortA, 10};
    constexpr microhal::IOPin txd{microhal::IOPin::PortB, 6};
}  // namespace a
namespace b {
    constexpr microhal::IOPin io1{microhal::IOPin::PortA, 0};
    constexpr microhal::IOPin io2{microhal::IOPin::PortA, 1};
    constexpr microhal::IOPin io3{microhal::IOPin::PortA, 4};
    constexpr microhal::IOPin io4{microhal::IOPin::PortA, 2};
    constexpr microhal::IOPin sda{microhal::IOPin::PortB, 11};
    constexpr microhal::IOPin scl{microhal::IOPin::PortC, 7};
    constexpr microhal::IOPin mosi{microhal::IOPin::PortB, 15};
    constexpr microhal::IOPin miso{microhal::IOPin::PortB, 14};
    constexpr microhal::IOPin sck{microhal::IOPin::PortC, 7};
    constexpr microhal::IOPin cs{microhal::IOPin::PortB, 12};
}  // namespace b
namespace c {
    constexpr microhal::IOPin io1{microhal::IOPin::PortB, 0};
    constexpr microhal::IOPin io2{microhal::IOPin::PortC, 2};
    constexpr microhal::IOPin io3{microhal::IOPin::PortC, 3};
    constexpr microhal::IOPin io4{microhal::IOPin::PortA, 3};
    constexpr microhal::IOPin sda{microhal::IOPin::PortC, 9};
    constexpr microhal::IOPin scl{microhal::IOPin::PortA, 8};
    constexpr microhal::IOPin mosi{microhal::IOPin::PortB, 5};
    constexpr microhal::IOPin miso{microhal::IOPin::PortB, 4};
    constexpr microhal::IOPin sck{microhal::IOPin::PortB, 3};
    constexpr microhal::IOPin cs{microhal::IOPin::PortC, 1};
}  // namespace c
}  // namespace con1
namespace con2 {
namespace a {
    constexpr microhal::IOPin io1{microhal::IOPin::PortC, 8};
    constexpr microhal::IOPin io2{microhal::IOPin::PortD, 8};
    constexpr microhal::IOPin io3{microhal::IOPin::PortB, 1};
    constexpr microhal::IOPin io4{microhal::IOPin::PortC, 6};
    constexpr microhal::IOPin io5{microhal::IOPin::PortC, 5};
    constexpr microhal::IOPin io6{microhal::IOPin::PortA, 9};
    constexpr microhal::IOPin sda{microhal::IOPin::PortB, 9};
    constexpr microhal::IOPin scl{microhal::IOPin::PortB, 8};
    constexpr microhal::IOPin mosi{microhal::IOPin::PortA, 7};
    constexpr microhal::IOPin miso{microhal::IOPin::PortA, 6};
    constexpr microhal::IOPin sck{microhal::IOPin::PortA, 5};
    constexpr microhal::IOPin cs{microhal::IOPin::PortB, 2};
    constexpr microhal::IOPin rxd{microhal::IOPin::PortA, 12};
    constexpr microhal::IOPin txd{microhal::IOPin::PortA, 11};
}  // namespace a


}  // namespace con2
}  // namespace bsp

#endif  // NUCLEO_F411RE_EXTENSION_V0_1_0_H_
