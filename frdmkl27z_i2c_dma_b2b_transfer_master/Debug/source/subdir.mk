################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c \
../source/MAX10302.c \
../source/TC2004_LCD.c \
../source/bpm_to_lcd.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/main.d \
./source/MAX10302.d \
./source/TC2004_LCD.d \
./source/bpm_to_lcd.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/main.o \
./source/MAX10302.o \
./source/TC2004_LCD.o \
./source/bpm_to_lcd.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\source" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\utilities" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\drivers" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\device" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\uart" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\lists" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\CMSIS" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\board" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\frdmkl27z\driver_examples\i2c\dma_b2b_transfer\master" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main.d ./source/main.o ./source/MAX10302.d ./source/MAX10302.o ./source/TC2004_LCD.d ./source/TC2004_LCD.o ./source/bpm_to_lcd.d ./source/bpm_to_lcd.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source
