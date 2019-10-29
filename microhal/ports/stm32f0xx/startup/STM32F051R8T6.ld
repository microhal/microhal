SEARCH_DIR(.);

/*
+=============================================================================+
| available memories definitions
+=============================================================================+
*/

MEMORY
{
	rom (rx)		: org = 0x08000000, len = 64k
	ram (rwx)		: org = 0x20000000, len = 8k	
}

INCLUDE STM32F0xx_memory_map.ld
