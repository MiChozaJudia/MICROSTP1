################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/SysTick.c \
../source/display.c \
../source/displayWrapper.c \
../source/door.c \
../source/encoder.c \
../source/fsm.c \
../source/fsmtable.c \
../source/gpio.c \
../source/reader.c \
../source/timer.c 

OBJS += \
./source/App.o \
./source/SysTick.o \
./source/display.o \
./source/displayWrapper.o \
./source/door.o \
./source/encoder.o \
./source/fsm.o \
./source/fsmtable.o \
./source/gpio.o \
./source/reader.o \
./source/timer.o 

C_DEPS += \
./source/App.d \
./source/SysTick.d \
./source/display.d \
./source/displayWrapper.d \
./source/door.d \
./source/encoder.d \
./source/fsm.d \
./source/fsmtable.d \
./source/gpio.d \
./source/reader.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


