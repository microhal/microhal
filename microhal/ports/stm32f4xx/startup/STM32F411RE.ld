SEARCH_DIR(.);

/*
+=============================================================================+
| available memories definitions
+=============================================================================+
*/

MEMORY
{
	rom (rx)		: org = 0x08000000, len = 512k
	ram (rwx)		: org = 0x20000000, len = 128k	
}

INCLUDE STM32F4xx_memory_map.ld
