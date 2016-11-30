/*
 * MAX5134.h
 *
 *  Created on: 20 cze 2014
 *      Author: Dell
 */

#ifndef MAX5134_H_
#define MAX5134_H_

class MAX5134 {
	/**
	 * @brief This enum contain possible commands.
	 */
	typedef enum{
		NOP = 0x00,	//<! No operation
		LDAC = 0x01, //<! Move contents of input to DAC registers
		CLR = 0x02, //<! Software clear.
		POWER_CONTROL = 0x03, //<! Power down DACs.
		LINEARITY = 0x05, //<! Optimize DAC linearity.
		WRITE = 0x10, //<! Write to selected input registers (DAC output not affected).
		WRITE_THROUGH = 0x30, //<! Write to selected input and DAC registers, DAC outputs updated (writethrough).
	} Command;
public:
	typedef enum{
		DAC1 = 0x01,
		DAC2 = 0x02,
		DAC3 = 0x04,
		DAC4 = 0x08
	} DACnumber;

	typedef enum{
		REFERENCE_VOLTAGE = 2440	//<! reference voltage in mV
	};

	MAX5134(microhal::SPI &spi, microhal::GPIO::IOPin CSPin, microhal::GPIO::IOPin ldacPin, microhal::GPIO::IOPin readyPin):spi(spi, CSPin),
			ldac(ldacPin, microhal::GPIO::OUTPUT), ready(readyPin, microhal::GPIO::INPUT){

	}
	virtual ~MAX5134(){

	}


	void update(){
		ldac.set();
		ldac.reset();
	}

	inline bool update(DACnumber dac){
		return update(dac, dac);
	}

	inline bool update(DACnumber dacA, DACnumber dacB){
		return update(dacA, dacB, dacB);
	}

	inline bool update(DACnumber dacA, DACnumber dacB, DACnumber dacC){
		return update(dacA, dacB, dacC, dacC);
	}

	bool update(DACnumber dacA, DACnumber dacB, DACnumber dacC, DACnumber dacD){
		struct __attribute__ ((packed)){
			uint8_t control;
			int16_t data;
		}toWrite;

		toWrite.control = LDAC | dacA | dacB| dacC | dacD;

		return spi.writeBuffer(&toWrite.control, sizeof(toWrite));
	}

	bool writeSample(DACnumber dac, uint16_t data){
		return spi.writeRegister(WRITE | dac, data, microhal::MSB_FIRST);
	}

	bool softwareClear(){
		struct __attribute__ ((packed)){
			uint8_t control;
			int16_t data;
		}toWrite;

		toWrite.control = CLR;

		return spi.writeBuffer(&toWrite.control, sizeof(toWrite));
	}
private:
	microhal::SPIDevice spi;
	microhal::GPIO ldac;
	microhal::GPIO ready;
};

#endif /* MAX5134_H_ */
