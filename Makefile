APP =		stm32g0

CROSS_COMPILE =	arm-none-eabi-
CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

export CROSS_COMPILE

OSDIR =		mdepx

all:
	python3 -B ${OSDIR}/tools/emitter.py mdepx.conf
	@${OBJCOPY} -O binary obj/${APP}.elf obj/${APP}.bin

clean:
	@rm -rf obj/*

flash:
	sudo openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c 'reset_config srst_only connect_assert_srst; program obj/${APP}.bin reset 0x08000000 exit'

include ${OSDIR}/mk/user.mk
