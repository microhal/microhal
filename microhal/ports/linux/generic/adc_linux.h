/*
 * adc_linux.h
 *
 *  Created on: Mar 18, 2022
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_LINUX_GENERIC_ADC_LINUX_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_LINUX_GENERIC_ADC_LINUX_H_

#include <filesystem>

#undef linux

namespace microhal {
namespace linux {

class ADC {
 public:
    ADC(std::filesystem::path adc_file_path) : adc_file_path(adc_file_path) {}

    int16_t get_value_raw() const noexcept;

 private:
    std::filesystem::path adc_file_path;
};

}  // namespace linux
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_LINUX_GENERIC_ADC_LINUX_H_ */
