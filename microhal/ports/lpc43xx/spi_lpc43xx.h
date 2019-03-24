#ifndef _MICROHAL_SPI_LPC43XX_H_
#define _MICROHAL_SPI_LPC43XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cmath>
#include <limits>
#include "IOPin.h"
//#include "clockManager.h"
#include "gpio_lpc43xx.h"
#include "interfaces/spi_interface.h"
#include "ssp.h"

namespace microhal {
class SPIDevice;
namespace lpc43xx {
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup lpc43xx
 * @{
 * @class SPI
 * @}
 * \brief   This class implements SPI functions.
 */
class SPI : public microhal::SPI {
 public:
    enum class Number { SSP0 = 0x40083000, SSP1 = 0x400C5000 };
    //---------------------------------------- functions ----------------------------------------//
    bool setMode(Mode mode) final {
        ssp.setSPIMode(mode);
        return true;
    }

    bool getMISOstate() final { return misoPin.get(); }

    bool isEnabled() { return ssp.isEnabled(); }

    void enable() final { ssp.enable(); }
    void disable() final { ssp.disable(); }

    void init() {}
    /**@brief Set SPI CLK frequency.
     *
     * @param speed - CLK frequency in Hz
     * @return speed value that was set, this is closest value that was possible to set with current bus speed.
     */
    uint32_t speed(uint32_t speed) final {
        ssp.baudrate(speed);
        return ssp.baudrate();
    }

    uint32_t speed() const final { return ssp.baudrate(); }

 protected:
    //---------------------------------------- variables ----------------------------------------//
    SSP &ssp;
    GPIO misoPin;
    //--------------------------------------- constructors --------------------------------------//
    SPI(SSP &spi, IOPin misoPin) : ssp(spi), misoPin(misoPin, GPIO::Direction::Input) {
        ssp.setFrameFormat(SSP::FrameFormat::SPI);
        ssp.configureAsMaster();
        ssp.setDataSize(8);
    }
    void busyWait() {
        while (!ssp.isTxFifoEmpty()) {
        }
        while (ssp.isBusy()) {
        }
    }

    //----------------------------------------- friends -----------------------------------------//
    friend microhal::SPIDevice;
};

}  // namespace lpc43xx
}  // namespace microhal

#endif  // _MICROHAL_SPI_LPC43XX_H_
