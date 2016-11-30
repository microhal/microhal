/* =========================================================================================== *//**
 @file        accelerometerMPU9150.h
 @authors     Paweł Okas
 @version     $Id$
 @package     HALFRED accelerometer implementation for MCP9150
 @brief       MPU9150 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis accelerometer.

 *//* ============================================================================================ */

#ifndef MPU9150ACCELEROMETER_H_
#define MPU9150ACCELEROMETER_H_

#include "units/Acceleration.h"
#include "math/vector.h"

class MPU9150;
/**
 * @addtogroup MPU9150
 * @{
 *      @class AccelerometerMPU9150
 * @}
 */
class AccelerometerMPU9150 {
    friend MPU9150;
public:
	/**
	 * @brief This enum describes the Full-Scale Range of accelerometer sensitivity
	 */
	typedef enum {
		RANGE_2g = 0,	///< Sensitivity +/- 2g
		RANGE_4g, 		///< Sensitivity +/- 4g
		RANGE_8g,		///< Sensitivity +/- 8g
		RANGE_16g,		///< Sensitivity +/- 16g
		UNKNOWN_RANGE	///< unknown range
	}FullScaleRange;

    /**
     * @brief Measured acceleration is returned as 3D vector of @ref Acceleration
     */
    using AccelerationVector = microhal::Vector<microhal::Acceleration, 3>;

    virtual ~AccelerometerMPU9150();

    bool reset(void);

    bool setRange(FullScaleRange range);

    FullScaleRange getRange(void);
    uint32_t getSamplingFrequency();

    /**
     * @brief Function reads acceleration measurement from device.
     *
     * @param [in] acc Reference to @ref AccelerationVector, where measurement will be stored.
     *  			If driver is unable to read measurements or they are not ready (for all configured axis),
     *  			result wont be writen.
     *
     * @return True if reading measurements was successful (all Axis values are obtained). False otherwise.
     *
     * @details Returned values of acceleration on each axis are express in g unit.
     *  		Conversion from read values to acceleration is performed according to current settings of range
     *  	    (using scale factors from datasheet).
     */
    bool getAcceleration(AccelerationVector& acc);
private:
    MPU9150 &mpu;
    FullScaleRange actualRange;
    AccelerationVector acceleration;

    AccelerometerMPU9150(MPU9150 *parent);

    void update(uint8_t *buffer);
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */


#endif /* MPU9150ACCELEROMETER_H_ */
