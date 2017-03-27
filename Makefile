CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-ld
OBJCPY:=arm-none-eabi-objcopy
OBJDUMP:=arm-none-eabi-objdump

LDFLAGS:=-march=armv6-m -mcpu=cortex-m0 -mthumb -nostartfiles -nostdlib
CFLAGS:=-march=armv6-m -mcpu=cortex-m0 -mthumb -Wall -fno-builtin -ffunction-sections -fdata-sections -nostdlib -std=c99
MACRO=-DCLIENT 

BLE_LIB_PATH=-L./Library/Bluetooth_LE/library
BLE_LIB=-lbluenrg1_stack_lib_atollic -lsmall_printf_atollic

HAL_INC=./Library/hal/inc
BLE_INC=./Library/Bluetooth_LE/inc
BLE_OTA_INC=./Library/Bluetooth_LE/OTA/inc
DRIVER_INC=./Library/BlueNRG1_Periph_Driver/inc
DEVICE_INC=./Library/CMSIS/Device/ST/BlueNRG1/Include
ARCH_INC=./Library/CMSIS/Include
SDK_INC=./Library/SDK_Eval_BlueNRG1/inc
APP_INC=./inc
OS_INC=./FreeRTOS/Source/include
OS_PORT_INC=./FreeRTOS/Source/portable/GCC/ARM_CM0

INCS=-I$(APP_INC) -I$(HAL_INC) -I$(BLE_INC) -I$(BLE_OTA_INC) -I$(DRIVER_INC) -I$(DEVICE_INC) -I$(ARCH_INC) -I$(SDK_INC) -I$(OS_INC) -I$(OS_PORT_INC)

SRC_HAL=$(wildcard *.c ./Library/hal/src/*.c) 
OBJS_HAL=$(patsubst %.c,%.o,$(SRC_HAL)) 
OBJS_HAL+=./Library/hal/src/context_switch.o 

#SRC_BLE_OTA=$(wildcard *.c ./Library/Bluetooth_LE/OTA/src/*.c)
#OBJS_BLE_OTA=$(patsubst %.c,%.o,$(SRC_BLE_OTA))

SRC_DRIVER=$(wildcard *.c ./Library/BlueNRG1_Periph_Driver/src/*.c)
OBJS_DRIVER=$(patsubst %.c,%.o,$(SRC_DRIVER))

SRC_DEVICE=$(wildcard *.c ./Library/CMSIS/Device/ST/BlueNRG1/Source/*.c)
OBJS_DEVICE=$(patsubst %.c,%.o,$(SRC_DEVICE))

SRC_SDK=$(wildcard *.c ./Library/SDK_Eval_BlueNRG1/src/*.c)
OBJS_SDK=$(patsubst %.c,%.o,$(SRC_SDK))

SRC_CMSIS=$(wildcard *.c ./Library/CMSIS/Source/*.c)
OBJS_CMSIS=$(patsubst %.c,%.o,$(SRC_CMSIS))

SRC_APP=$(wildcard *.c ./src/*.c)
OBJS_APP=$(patsubst %.c,%.o,$(SRC_APP))

SRC_OS=$(wildcard *.c ./FreeRTOS/Source/*.c)
OBJS_OS=$(patsubst %.c,%.o,$(SRC_OS))
OBJS_OS+=./FreeRTOS/Source/portable/MemMang/heap_1.o \
         ./FreeRTOS/Source/portable/GCC/ARM_CM0/port.o
#.c.i:
#	$(CC) -E $< $(CFLAGS) $(INCS) -o $@

.PHONY: bleapp clean
bleapp:$(OBJS_APP) $(OBJS_HAL) $(OBJS_DRIVER) $(OBJS_DEVICE) $(OBJS_SDK) $(OBJS_CMSIS) $(OBJS_OS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS_APP) $(OBJS_HAL) $(OBJS_DRIVER) $(OBJS_DEVICE) $(OBJS_SDK) $(OBJS_CMSIS) $(OBJS_OS) $(BLE_LIB_PATH) $(BLE_LIB) -Wl,-T./ldscript/BlueNRG1.ld -Wl,-Map=$@.map -Wl,--gc-sections
	$(OBJCPY) -O binary -S $@ $@.bin
	$(OBJCPY) -O ihex -S $@ $@.hex
	$(OBJDUMP) -D $@ > $@.s


.c.o:
	$(CC) -c $< $(CFLAGS) $(MACRO) $(INCS) -o $@
.S.i:
	$(CC) -E $< $(CFLAGS) $(INCS) -o $@
.i.o:
	$(CC) -c $< $(CFLAGS) $(INCS) -o $@

clean:
	rm -rf $(OBJS_APP) $(OBJS_HAL) $(OBJS_DRIVER) $(OBJS_DEVICE) $(OBJS_SDK) $(OBJS_CMSIS) $(OBJS_OS) bleapp bleapp.map bleapp.bin bleapp.hex bleapp.s

