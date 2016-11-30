/** \file itoa.c
 * \brief itoa() function.
 * \details itoa() function to convert integer to string.
 * \author Freddie Chopin  modify by Pawel Okas
 * \date 2009-07-13
 */

/******************************************************************************
 * project: 750-750
 * chip: STM32F103VB
 * compiler: arm-none-eabi-gcc 4.4.1
 *
 * prefix: (none)
 *
 * available global functions:
 * 	char* itoa(int value, char* buffer, int base)
 *
 * available local functions:
 * 	static void __reverse(char* begin,char* end);
 ******************************************************************************/

/*
 +=============================================================================+
 | includes
 +=============================================================================+
 */

#include <stdint.h>

#include "itoa.h"

/*
 +=============================================================================+
 | module variables
 +=============================================================================+
 */

/*
 +=============================================================================+
 | local functions' declarations
 +=============================================================================+
 */

static void __reverse(char* begin, char* end);
static char* itoa_base(uint64_t value, char* buffer, int base);
static char* itoa_base(uint32_t value, char* buffer, int base);
static const char digits[] = "0123456789ABCDEF";

/*
 +=============================================================================+
 | global functions
 +=============================================================================+
 */

/*------------------------------------------------------------------------*//**
 * \brief itoa() function.
 * \details Converts signed integer to an char array. Valid 'base' in [2;16].
 * Only base == 10 values are treated as signed
 *
 * \param [in] value the value which will be converted
 * \param [out] buffer the output buffer
 * \param [in] base the base of conversion
 * \return pointer to \em buffer
 *//*-------------------------------------------------------------------------*/
char* itoa(uint64_t value, char* buffer, int base){
	char* buffer_end = buffer;

	if ((base >= 2) && (base <= 16)){				// is the base valid?
		buffer_end = itoa_base(value, buffer, base);
		__reverse(buffer, buffer_end - 1);		// reverse the string
	}

	*buffer_end = '\0';
	return buffer;
}

char* itoa(uint32_t value, char* buffer, int base){
	char* buffer_end = buffer;

	if ((base >= 2) && (base <= 16)) {				// is the base valid?

		buffer_end = itoa_base(static_cast<uint32_t>(value), buffer, base);

		__reverse(buffer, buffer_end - 1);		// reverse the string
	}

	*buffer_end = '\0';
	return buffer;
}

char* itoa(int64_t value, char* buffer, int base) {
	int32_t sign = 0;
	char* buffer_end = buffer;

	if ((base >= 2) && (base <= 16)){				// is the base valid?
		if (base == 10 && value < 0){// negative value and base == 10? store the copy (sign)
			value = -value;
			sign = -1; // make it positive
		}

		buffer_end = itoa_base(static_cast<uint64_t>(value), buffer, base);

		if (sign < 0)							// was the value negative?
			*buffer_end++ = '-';					// append the sign

		__reverse(buffer, buffer_end - 1);		// reverse the string
	}

	*buffer_end = '\0';
	return buffer;
}

char* itoa(int32_t value, char* buffer, int base) {
	int32_t sign = 0;
	char* buffer_end = buffer;

	if ((base >= 2) && (base <= 16)){				// is the base valid?
		if (base == 10 && (sign = value) < 0){// negative value and base == 10? store the copy (sign)
			value = -value; // make it positive
		}

		buffer_end = itoa_base(static_cast<uint32_t>(value), buffer, base);

		if (sign < 0)							// was the value negative?
			*buffer_end++ = '-';					// append the sign

		__reverse(buffer, buffer_end - 1);		// reverse the string
	}

	*buffer_end = '\0';
	return buffer;
}

char* itoa_base(uint32_t value, char* buffer, int base){
	uint32_t quot, rem;

	do {
		quot = value / base;			// calculate quotient and remainder
		rem = value % base;
		*buffer++ = digits[rem];	// append the remainder to the string
	} while ((value = quot));	// loop while there is something to convert

	return buffer;
}

char* itoa_base(uint64_t value, char* buffer, int base){
	uint64_t quot;
	uint32_t rem;


	do {
		quot = value / base;			// calculate quotient and remainder
		rem = value % base;
		*buffer++ = digits[rem];	// append the remainder to the string
	} while ((value = quot));	// loop while there is something to convert

	return buffer;
}

/*
 +=============================================================================+
 | local functions
 +=============================================================================+
 */

/*------------------------------------------------------------------------*//**
 * \brief Reverse order in string.
 * \details Reverses the order of the elements in string.
 *
 * \param [in,out] begin pointer to the beginning of the string
 * \param [in,out] end pointer to the end of the string
 *//*-------------------------------------------------------------------------*/

static void __reverse(char* begin, char* end) {
	char temp;

	while (end > begin) {
		temp = *end;
		*end-- = *begin;
		*begin++ = temp;
	}
}

/******************************************************************************
 * END OF FILE
 ******************************************************************************/
