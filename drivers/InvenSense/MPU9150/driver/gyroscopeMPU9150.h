/* =========================================================================================== *//**
 @file        GyroscopeMPU9150.h
 @authors     Pawe≈Ç Okas
 @version     $Id$
 @package     HALFRED gyroscope implementation for MCP9150
 @brief       MPU9150 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis gyroscope.

 *//* ============================================================================================ */

#ifndef GYROSCOPEMPU9150_H_
#define GYROSCOPEMPU9150_H_

#include <cstdint>
#include "units/angularVelocity.h"
#include "math/vector.h"

class MPU9150;
/**
 * @addtogroup MPU9150
 * @{
 * @class GyroscopeMPU9150
 * @}
 */
class GyroscopeMPU9150 {
	friend MPU9150;
public:
    /**
     * @brief Possible gyroscope Full-Scale ranges in ∞/sec.
     */
    enum FullScaleRange {
        RANGE_250 = 0x00,  //!< Sensitivity +/- 250
        RANGE_500 = 0x01,  //!< Sensitivity +/- 500
        RANGE_1000 = 0x02, //!< Sensitivity +/- 1000
        RANGE_2000 = 0x03, //!< Sensitivity +/- 2000
        UNKNOWN_RANGE      //!< UNKNOWN_RANGE is used to indication of error
    };

    using AngularVelocityVector = microhal::Vector<microhal::AngularVelocity, 3>;

	virtual ~GyroscopeMPU9150();
	bool setRange(FullScaleRange range);
	bool reset(void);
	FullScaleRange getRange(void);
	uint32_t getSamplingFrequency();
private:
	MPU9150 &mpu;
	FullScaleRange actualRange;
	AngularVelocityVector angularVelocity;

	GyroscopeMPU9150(MPU9150 *parent);

	void update(uint8_t *buffer);
};


#endif /* GYROSCOPEMPU9150_H_ */
