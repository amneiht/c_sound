################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/g711/g711.c \
../src/g711/g711Array.c 

OBJS += \
./src/g711/g711.o \
./src/g711/g711Array.o 

C_DEPS += \
./src/g711/g711.d \
./src/g711/g711Array.d 


# Each subdirectory must supply rules for building sources it contributes
src/g711/%.o: ../src/g711/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


