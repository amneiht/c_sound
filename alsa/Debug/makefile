################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/test/old/subdir.mk
-include src/test/subdir.mk
-include src/support/subdir.mk
-include src/g711/subdir.mk
-include src/asocket/rtp/subdir.mk
-include src/asocket/subdir.mk
-include src/asip/subdir.mk
-include src/app/subdir.mk
-include src/am_audio/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: alsa

# Tool invocations
alsa: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "alsa" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) alsa
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
