/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Driver for mpl115a1 barometr

 * @authors    Marcin Urbanik
 * created on: 18-01-2016
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2016, microHAL
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mpl115a1.h"




bool mpl115a1::mpl115a1ReadPressureAndTempADC()
{

	uint8_t reg;
	//Send command to convert pressure and temperature
	if(0==writeRegister( CONVERT << 1 , 0x00)) return 1;
	//readRegister( WRITE | (CONVERT << 1), reg);

	std::this_thread::sleep_for(std::chrono::milliseconds{3});	// wait for A-D conversion to take place
	//Read converted values
	if(0==readRegister(READ | (Padc_MSB  << 1), mpl115a1Pressure[0]))return 1;
	if(0==readRegister(READ | (Padc_LSB  << 1), mpl115a1Pressure[1]))return 1;
	if(0==readRegister(READ | (Tadc_MSB  << 1), mpl115a1Temperature[0]))return 1;
	if(0==readRegister(READ | (Tadc_LSB  << 1), mpl115a1Temperature[1]))return 1;



	return 0;
}
bool mpl115a1::mpl115a1ReadCoeffs()
{
//	uint8_t mpl115a1Coeffs[12];
	if(0==readRegister(READ | (a0_MSB << 1), mpl115a1Coeffs[0]))return 1;
	if(0==readRegister(READ | (a0_LSB << 1), mpl115a1Coeffs[1]))return 1;
	if(0==readRegister(READ | (b1_MSB << 1), mpl115a1Coeffs[2]))return 1;
	if(0==readRegister(READ | (b1_LSB << 1), mpl115a1Coeffs[3]))return 1;
	if(0==readRegister(READ | (b2_MSB << 1), mpl115a1Coeffs[4]))return 1;
	if(0==readRegister(READ | (b2_LSB << 1), mpl115a1Coeffs[5]))return 1;
	if(0==readRegister(READ | (c12_MSB << 1), mpl115a1Coeffs[6]))return 1;
	if(0==readRegister(READ | (c12_LSB << 1), mpl115a1Coeffs[7]))return 1;
	mpl115a1Coeffs[8]=0x00;//reserved, always 0x00;
	mpl115a1Coeffs[9]=0x00;
	mpl115a1Coeffs[10]=0x00;
	mpl115a1Coeffs[11]=0x00;

	uint8_t sia0MSB, sia0LSB, sib1MSB, sib1LSB, sib2MSB, sib2LSB, sic12MSB, sic12LSB, sic11MSB, sic11LSB, sic22MSB, sic22LSB;// Temporary variables

	//Pressure calculation
	sia0MSB = mpl115a1Coeffs[0];
	sia0LSB = mpl115a1Coeffs[1];
	sia0 = static_cast<int16_t>(sia0MSB <<8); 					//s16 type //Shift to MSB
	sia0 += static_cast<int16_t>(sia0LSB & 0x00FF); 			//Add LSB to 16bit number
	//coeff b1 16bit
	sib1MSB= mpl115a1Coeffs[2];
	sib1LSB= mpl115a1Coeffs[3];
	sib1 = sib1MSB <<8; 						//Shift to MSB
	sib1 += sib1LSB & 0x00FF; 					//Add LSB to 16bit number
	//coeff b2 16bit
	sib2MSB= mpl115a1Coeffs[4];
	sib2LSB= mpl115a1Coeffs[5];
	sib2 = sib2MSB <<8;							//Shift to MSB
	sib2 += sib2LSB & 0x00FF; 					//Add LSB to 16bit number
	//coeff c12 14bit
	sic12MSB= mpl115a1Coeffs[6];
	sic12LSB= mpl115a1Coeffs[7];
	sic12 = sic12MSB <<8; 						//Shift to MSB only by 8 for MSB
	sic12 += sic12LSB & 0x00FF;
	//coeff c11 11bit
	sic11MSB= mpl115a1Coeffs[8];
	sic11LSB= mpl115a1Coeffs[9];
	sic11 = sic11MSB <<8; 						//Shift to MSB only by 8 for MSB
	sic11 += sic11LSB & 0x00FF;
	//coeff c22 11bit
	sic22MSB= mpl115a1Coeffs[10];
	sic22LSB= mpl115a1Coeffs[11];
	sic22 = sic22MSB <<8; 						//Shift to MSB only by 8 for MSB
	sic22 += sic22LSB & 0x00FF;



	return 0;
}

void mpl115a1::mpl115a1CalculatePressure(double *CalcPress)
{

	unsigned int uiPadc, PressCntdec, uiTadc, TempCntdec, siPcomp;
	int32_t lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2, si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
	double decPcomp;



	PressCntdec = (mpl115a1Pressure[0] << 8) | mpl115a1Pressure[1];
	TempCntdec = (mpl115a1Temperature[0] << 8) | mpl115a1Temperature[1];

	uiPadc=PressCntdec>>6; 						//Note that the PressCntdec is the raw value from the MPL115A data address. Its shifted >>6 since its 10 bit.
	uiTadc=TempCntdec>>6;						//Note that the TempCntdec is the raw value from the MPL115A data address. Its shifted >>6 since its 10 bit.
	//******* STEP 1 c11x1= c11 * Padc
	lt1 = static_cast<int32_t>(sic11); 						// s(16,27) s(N,F+zeropad) goes from s(11,10)+11ZeroPad = s(11,22) => Left Justified = s(16,27)
	lt2 = static_cast<int32_t>(uiPadc); 						// u(10,0) s(N,F)
	lt3 = lt1 * lt2; 							// s(26,27) /c11*Padc
	si_c11x1 = static_cast<int32_t>(lt3); 					// s(26,27)- EQ 1 =c11x1 /checked
	//divide this hex number by 2^30 to get the correct decimal value.
	//b1 =s(14,11) => s(16,13) Left justified
	//******* STEP 2 a11= b1 + c11x1
	lt1 = (static_cast<int32_t>(sib1)<<14); 					// s(30,27) b1=s(16,13) Shift b1 so that the F matches c11x1(shift by 14)
	lt2 = static_cast<int32_t>(si_c11x1); 					// s(26,27) //ensure fractional bits are compatible
	lt3 = lt1 + lt2; 							// s(30,27) /b1+c11x1
	si_a11 = static_cast<int32_t>((lt3)>>14); 				// s(16,13) - EQ 2 =a11 Convert this block back to s(16,X)
	//******* STEP 3 c12x2= c12 * Tadc
	// sic12 is s(14,13)+9zero pad = s(16,15)+9 => s(16,24) left justified
	lt1 = static_cast<int32_t>(sic12); 						// s(16,24)
	lt2 = static_cast<int32_t>(uiTadc); 						// u(10,0)
	lt3 = lt1 * lt2; 							// s(26,24)
	si_c12x2 = static_cast<int32_t>(lt3); 					// s(26,24) - EQ 3 =c12x2 /checked
	//******* STEP 4 a1= a11 + c12x2
	lt1 = (static_cast<int32_t>(si_a11<<11)); 				// s(27,24) This is done by s(16,13) <<11 goes to s(27,24) to match c12x2's F part
	lt2 = static_cast<int32_t>(si_c12x2); 					// s(26,24)
	lt3 = lt1 + lt2; 							// s(27,24) /a11+c12x2
	si_a1 =static_cast<int32_t>((lt3>>11)); 					// s(16,13) - EQ 4 =a1 /check
	//******* STEP 5 c22x2= c22 * Tadc
	// c22 is s(11,10)+9zero pad = s(11,19) => s(16,24) left justified
	lt1 = static_cast<int32_t>(sic22); 						// s(16,30) This is done by s(11,10) + 15 zero pad goes to s(16,15)+15, to s(16,30)
	lt2 = static_cast<int32_t>(uiTadc); 						// u(10,0)
	lt3 = lt1 * lt2; 							// s(26,30) /c22*Tadc
	si_c22x2 = static_cast<int32_t>((lt3)); 					// s(26,30) - EQ 5 /=c22x2
	//******* STEP 6 a2= b2 + c22x2
	//WORKS and loses the least in data. One extra execution. Note how the 31 is really a 32 due to possible overflow.
	// b2 is s(16,14) User shifted left to => s(31,29) to match c22x2 F value
	lt1 = (static_cast<int32_t>(sib2<<15)); 					//s(31,29)

	lt2 = (static_cast<int32_t>(si_c22x2>>1)); 				//s(25,29) s(26,30) goes to >>16 s(10,14) to match F from sib2
	lt3 = lt1+lt2; 								//s(32,29) but really is a s(31,29) due to overflow the 31 becomes a 32.
	si_a2 = (static_cast<int32_t>(lt3>>16)); 				//s(16,13)
	//******* STEP 7 a1x1= a1 * Padc
	lt1 = static_cast<int32_t>(si_a1); 						// s(16,13)
	lt2 = static_cast<int32_t>(uiPadc); 						// u(10,0)
	lt3 = lt1 * lt2; 							// s(26,13) /a1*Padc
	si_a1x1 = static_cast<int32_t>((lt3)); 					// s(26,13) - EQ 7 /=a1x1 /check
	//******* STEP 8 y1= a0 + a1x1
	// a0 = s(16,3)
	lt1 = (static_cast<int32_t>(sia0<<10)); 					// s(26,13) This is done since has to match a1x1 F value to add. So S(16,3) <<10 = S(26,13)
	lt2 = static_cast<int32_t>(si_a1x1); 					// s(26,13)
	lt3 = lt1 + lt2; 							// s(26,13) /a0+a1x1
	si_y1 = (static_cast<int32_t>(lt3>>10)); // s(16,3) - EQ 8 /=y1 /check
	//******* STEP 9 a2x2= a2 *Tadc
	lt1 = static_cast<int32_t>(si_a2); 						// s(16,13)
	lt2 = static_cast<int32_t>(uiTadc);						// u(10,0)
	lt3 = lt1 * lt2; 							// s(26,13) /a2*Tadc
	si_a2x2 = static_cast<int32_t>((lt3)); 					// s(26,13) - EQ 9 /=a2x2
	//******* STEP 10 pComp = y1 +a2x2
	// y1= s(16,3)
	lt1 = (static_cast<int32_t>(si_y1<<10)); 				// s(26,13) This is done to match a2x2 F value so addition can match. s(16,3) <<10
	lt2 = static_cast<int32_t>(si_a2x2); 					// s(26,13)
	lt3 = lt1 + lt2; 							// s(26,13) /y1+a2x2
	// FIXED POINT RESULT WITH ROUNDING:
	siPcomp = ((int16_t)(lt3>>13)); 			// goes to no fractional parts since this is an ADC count.
	//decPcomp is defined as a floating point number.
	//Conversion to Decimal value from 1023 ADC count value. ADC counts are 0 to 1023. Pressure is 50 to 115kPa correspondingly.
	decPcomp = ((65.0/1023.0)*siPcomp)+50.0;
	*CalcPress = decPcomp;
}
