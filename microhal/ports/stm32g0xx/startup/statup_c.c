/*
 * statup_c.c
 *
 *  Created on: Feb 3, 2021
 *      Author: pokas
 */
#include <stdint.h>
#include <string.h>
#include "../nvic.h"

extern void (*const vectors[])(void);

void startup_code(void) {
    SCB->VTOR = (uint32_t)&vectors;
    extern char __data_start;  // imported from linker script
    extern char __data_end;    // imported from linker script
    extern char __data_init_start;

    memcpy(&__data_start, &__data_init_start, &__data_end - &__data_start);

    // init bss section
    extern char __bss_start;
    extern char __bss_end;
    memset(&__bss_start, 0, &__bss_end - &__bss_start);
}
