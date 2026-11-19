################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_dma.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_dma.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_dma.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_dma.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_dma.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_dma.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\source" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\utilities" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\drivers" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\device" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\uart" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\lists" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\CMSIS" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\board" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\frdmkl27z\driver_examples\i2c\dma_b2b_transfer\master" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_dma.d ./drivers/fsl_dma.o ./drivers/fsl_dmamux.d ./drivers/fsl_dmamux.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_i2c.d ./drivers/fsl_i2c.o ./drivers/fsl_i2c_dma.d ./drivers/fsl_i2c_dma.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_smc.d ./drivers/fsl_smc.o ./drivers/fsl_uart.d ./drivers/fsl_uart.o

.PHONY: clean-drivers

