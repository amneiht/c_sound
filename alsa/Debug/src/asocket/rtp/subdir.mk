################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/asocket/rtp/send.c 

OBJS += \
./src/asocket/rtp/send.o 

C_DEPS += \
./src/asocket/rtp/send.d 


# Each subdirectory must supply rules for building sources it contributes
src/asocket/rtp/%.o: ../src/asocket/rtp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


