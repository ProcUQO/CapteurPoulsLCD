################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL27Z64VLH4 -DCPU_MKL27Z64VLH4_cm0plus -DFRDM_KL27Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\source" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\utilities" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\drivers" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\device" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\uart" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\component\lists" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\CMSIS" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\board" -I"C:\Users\user\Documents\MCUXpressoIDE_25.6.136\workspace\frdmkl27z_i2c_dma_b2b_transfer_master\frdmkl27z\driver_examples\i2c\dma_b2b_transfer\master" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o

.PHONY: clean-utilities

