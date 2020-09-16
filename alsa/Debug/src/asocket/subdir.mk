################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/asocket/sipclient.c \
../src/asocket/sock.c 

OBJS += \
./src/asocket/sipclient.o \
./src/asocket/sock.o 

C_DEPS += \
./src/asocket/sipclient.d \
./src/asocket/sock.d 


# Each subdirectory must supply rules for building sources it contributes
src/asocket/%.o: ../src/asocket/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


