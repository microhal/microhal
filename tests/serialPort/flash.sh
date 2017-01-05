#!/bin/bash

BINARY=stm32f4-discovery/serialPort_test.elf

OPENOCD_PATH=/home/pawel/Pulpit/microide_test/microide/tools/openocd/0.10.0/bin
OPENOCD=$OPENOCD_PATH/openocd


openocdFlash() {
$OPENOCD -f board/$1 \
-c init \
-c targets \
-c reset \
-c halt \
-c "sleep 100" \
-c "flash write_image erase $2" \
-c "verify_image $2" \
-c "reset run" \
-c shutdown
}



case $1 in
	stm32f4-discovery)
		openocdFlash stm32f4discovery.cfg $BINARY
		;;
	nucleo-l476rg)
		openocdFlash stm32l4discovery.cfg $BINARY
		;;
	*)
		echo $"Usage: $0 {board name} {flash file}"
		exit 1
	
esac
