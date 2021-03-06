LIB_DIR = STM32Cube_FW_F4_V1.9.0

DRIVER = $(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Src

BUILD_DIR = build


#Add any source files that you use here. Remember to end them with .o
OBJECTS = main.o        \
	src/stm32f4xx_it.o  \
	src/print.o         \
	src/packet_in.o      \
	src/packet_out.o     \
	src/init.o          \
	src/pressure.o      \
	src/servo.o         \
	src/matrices.o      \
	src/pi_controller.o \
	src/imu.o


OBJECTS +=                                                                                  \
	$(DRIVER)/stm32f4xx_hal.o                                                               \
	$(DRIVER)/stm32f4xx_hal_adc.o                                                           \
	$(DRIVER)/stm32f4xx_hal_crc.o                                                           \
	$(DRIVER)/stm32f4xx_hal_can.o                                                           \
	$(DRIVER)/stm32f4xx_hal_uart.o                                                          \
	$(DRIVER)/stm32f4xx_hal_usart.o                                                         \
	$(DRIVER)/stm32f4xx_hal_cortex.o                                                        \
	$(DRIVER)/stm32f4xx_hal_gpio.o                                                          \
	$(DRIVER)/stm32f4xx_hal_rcc.o                                                           \
	$(DRIVER)/stm32f4xx_hal_dma.o                                                           \
	$(DRIVER)/stm32f4xx_hal_spi.o                                                           \
	$(DRIVER)/stm32f4xx_hal_i2c.o                                                           \
	$(DRIVER)/stm32f4xx_hal_i2c_ex.o                                                        \
	$(DRIVER)/stm32f4xx_hal_tim.o                                                           \
	$(DRIVER)/stm32f4xx_hal_tim_ex.o                                                        \
	$(DRIVER)/stm32f4xx_hal_i2s.o                                                           \
	$(DRIVER)/stm32f4xx_hal_i2s_ex.o                                                        \
	$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o        \
	$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.o \
	$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o        \
	$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/iar/startup_stm32f407xx.s \


INCLUDE_PATHS =                                              \
	-I./inc                                                  \
	-I./$(LIB_DIR)/Inc                                       \
	-I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc          \
	-I./$(LIB_DIR)/Drivers/CMSIS/Include                     \
	-I./$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	-I./$(LIB_DIR)/Middlewares/Third_party/LwIP/test/unit    \
	-I./$(LIB_DIR)/Middlewares/Third_party/LwIP/system       \
	-I./$(LIB_DIR)/Middlewares/Third_Party/LwIP/system       \
	-I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc          \
	-I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy   \
	-I./$(LIB_DIR)/Middlewares/Third_Party/LwIP/system/arch  \


LINKER_SCRIPT = ./$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/STM32F407XG.ld


#Build Directory: Change if useing a different folder
BUILD_DIR = build

#Project Name: This name gets used to generate the .o, .bin, and .hex files
PROJECT = main


###############################################################################
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE    = $(GCC_BIN)arm-none-eabi-size

# Compiling flags needed by the boards
CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CC_FLAGS = $(CPU) -c -g -w -fno-common -fmessage-length=0 -Wall -Wextra -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP -fdiagnostics-color=auto
CC_SYMBOLS =            \
        -DSTM32F407xx   \
        -DSTM32F4       \
        -DSTM32F407VG

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs -u _printf_float -u _scanf_float -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

CC_FLAGS += -DNDEBUG -Os

.PHONY: all compile clean burn dfu test

all: compile

compile: $(PROJECT).bin $(PROJECT).hex
	@echo " ==> Compiled $(PROJECT).bin & $(PROJECT).hex"

.c.o:
	@$(CC) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	@$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<


$(LIB_OBJ): $(LIB_SRC)
	@$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) $(LIB_SRC)
	@mv *.o ./$(BUILD_DIR)

$(PROJECT).elf: $(OBJECTS)
	@$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) -o $(BUILD_DIR)/$@ $^ $(LD_SYS_LIBS) $(LD_SYS_LIBS)
	@rm *.o
	@rm src/*.o

$(PROJECT).bin: $(PROJECT).elf
	@rm *.d
	@rm src/*.d
	@$(OBJCOPY) -O binary $(BUILD_DIR)/$< $(BUILD_DIR)/$@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -g -O ihex $(BUILD_DIR)/$< $(BUILD_DIR)/$@


clean:
	@rm $(BUILD_DIR)/*.hex
	@rm $(BUILD_DIR)/*.elf
	@rm $(BUILD_DIR)/*.map
	@rm $(BUILD_DIR)/*.bin
	@echo " ==> Cleaned build directory"

burn:
	@echo " ==> Flashing $(BUILD_DIR)/main.bin to device with st-flash...\n"
	@st-flash --reset write $(BUILD_DIR)/main.bin 0x8000000

dfu:
	@echo " ==> Flashing $(BUILD_DIR)/$(PROJECT).bin to device with dfu-util...\n"
	@dfu-util -a 0 --dfuse-address 0x08000000 -D $(BUILD_DIR)/$(PROJECT).bin
