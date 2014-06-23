################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/data_package.c \
../src/db_interface.c \
../src/logger.c \
../src/main.c \
../src/plc_interface.c 

OBJS += \
./src/data_package.o \
./src/db_interface.o \
./src/logger.o \
./src/main.o \
./src/plc_interface.o 

C_DEPS += \
./src/data_package.d \
./src/db_interface.d \
./src/logger.d \
./src/main.d \
./src/plc_interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/urs/include -includerrd.h -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


