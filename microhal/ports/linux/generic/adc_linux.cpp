/*
 * adc_linux.cpp
 *
 *  Created on: Mar 18, 2022
 *      Author: pokas
 */

#include "adc_linux.h"
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <charconv>
#include <cstdio>
#include "diagnostic/diagnostic.h"

namespace microhal {
namespace linux {

int16_t ADC::get_value_raw() const noexcept {
    if (!adc_file_path.empty()) {
        auto fd = open(adc_file_path.c_str(), O_RDONLY);
        if (fd > 0) {
            char value[8];
            ssize_t len = read(fd, &value, sizeof(value));
            close(fd);

            int int_value;
            auto result = std::from_chars(value, value + len, int_value);
            if (result.ec == std::errc()) {
                return int_value;
            }
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "ADC error, unable to open ADC file: " << adc_file_path.c_str() << microhal::diagnostic::unlock;
            std::terminate();
        }
    }
    return 0;
    // return Error::LinuxADCUnconfigured;
}

}  // namespace linux
}  // namespace microhal
