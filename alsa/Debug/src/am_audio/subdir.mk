################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/am_audio/sound.c 

OBJS += \
./src/am_audio/sound.o 

C_DEPS += \
./src/am_audio/sound.d 


# Each subdirectory must supply rules for building sources it contributes
src/am_audio/%.o: ../src/am_audio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


