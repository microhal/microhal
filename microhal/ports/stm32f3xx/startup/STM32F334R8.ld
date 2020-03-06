SEARCH_DIR(.);

/*
+=============================================================================+
| available memories definitions
+=============================================================================+
*/

MEMORY
{
	rom (rx)		: org = 0x08000000, len = 64k
	ram (rwx)		: org = 0x20000000, len = 12k
	ccm_ram (rwx)   : org = 0x10000000, len = 4k
}

INCLUDE STM32F3xx_memory_map.ld
