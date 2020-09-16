################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/asip/extract.c \
../src/asip/variable.c 

OBJS += \
./src/asip/extract.o \
./src/asip/variable.o 

C_DEPS += \
./src/asip/extract.d \
./src/asip/variable.d 


# Each subdirectory must supply rules for building sources it contributes
src/asip/%.o: ../src/asip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


