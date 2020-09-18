################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/test/old/TestPluse.c \
../src/test/old/TestPulse.c \
../src/test/old/TestRtp.c \
../src/test/old/TestServer.c \
../src/test/old/TestThread.c \
../src/test/old/Testalsa.c \
../src/test/old/testSocketClose.c \
../src/test/old/testStream.c 

OBJS += \
./src/test/old/TestPluse.o \
./src/test/old/TestPulse.o \
./src/test/old/TestRtp.o \
./src/test/old/TestServer.o \
./src/test/old/TestThread.o \
./src/test/old/Testalsa.o \
./src/test/old/testSocketClose.o \
./src/test/old/testStream.o 

C_DEPS += \
./src/test/old/TestPluse.d \
./src/test/old/TestPulse.d \
./src/test/old/TestRtp.d \
./src/test/old/TestServer.d \
./src/test/old/TestThread.d \
./src/test/old/Testalsa.d \
./src/test/old/testSocketClose.d \
./src/test/old/testStream.d 


# Each subdirectory must supply rules for building sources it contributes
src/test/old/%.o: ../src/test/old/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


