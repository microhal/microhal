#include "FreeRTOS.h"
#include "task.h"

extern "C" __attribute__((weak)) void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
    (void)pcTaskName;
    (void)pxTask;

    /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

extern "C" __attribute__((weak)) void vApplicationIdleHook(void) {
    while (1) {
    }
}
