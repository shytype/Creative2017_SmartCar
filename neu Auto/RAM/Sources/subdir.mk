################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Exceptions.c" \
"../Sources/IntcInterrupts.c" \
"../Sources/L3G4200D.c" \
"../Sources/RFIDCard.c" \
"../Sources/UART.c" \
"../Sources/WiFi.c" \
"../Sources/action.c" \
"../Sources/analyse.c" \
"../Sources/camcontrol.c" \
"../Sources/control.c" \
"../Sources/diskio.c" \
"../Sources/drifting.c" \
"../Sources/ff.c" \
"../Sources/init.c" \
"../Sources/ivor_branch_table.c" \
"../Sources/lcd.c" \
"../Sources/mag.c" \
"../Sources/main.c" \
"../Sources/sdcard.c" \
"../Sources/supersonic.c" \
"../Sources/time.c" \
"../Sources/video.c" \

C_SRCS += \
../Sources/Exceptions.c \
../Sources/IntcInterrupts.c \
../Sources/L3G4200D.c \
../Sources/RFIDCard.c \
../Sources/UART.c \
../Sources/WiFi.c \
../Sources/action.c \
../Sources/analyse.c \
../Sources/camcontrol.c \
../Sources/control.c \
../Sources/diskio.c \
../Sources/drifting.c \
../Sources/ff.c \
../Sources/init.c \
../Sources/ivor_branch_table.c \
../Sources/lcd.c \
../Sources/mag.c \
../Sources/main.c \
../Sources/sdcard.c \
../Sources/supersonic.c \
../Sources/time.c \
../Sources/video.c \

OBJS += \
./Sources/Exceptions_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/L3G4200D_c.obj \
./Sources/RFIDCard_c.obj \
./Sources/UART_c.obj \
./Sources/WiFi_c.obj \
./Sources/action_c.obj \
./Sources/analyse_c.obj \
./Sources/camcontrol_c.obj \
./Sources/control_c.obj \
./Sources/diskio_c.obj \
./Sources/drifting_c.obj \
./Sources/ff_c.obj \
./Sources/init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/lcd_c.obj \
./Sources/mag_c.obj \
./Sources/main_c.obj \
./Sources/sdcard_c.obj \
./Sources/supersonic_c.obj \
./Sources/time_c.obj \
./Sources/video_c.obj \

OBJS_QUOTED += \
"./Sources/Exceptions_c.obj" \
"./Sources/IntcInterrupts_c.obj" \
"./Sources/L3G4200D_c.obj" \
"./Sources/RFIDCard_c.obj" \
"./Sources/UART_c.obj" \
"./Sources/WiFi_c.obj" \
"./Sources/action_c.obj" \
"./Sources/analyse_c.obj" \
"./Sources/camcontrol_c.obj" \
"./Sources/control_c.obj" \
"./Sources/diskio_c.obj" \
"./Sources/drifting_c.obj" \
"./Sources/ff_c.obj" \
"./Sources/init_c.obj" \
"./Sources/ivor_branch_table_c.obj" \
"./Sources/lcd_c.obj" \
"./Sources/mag_c.obj" \
"./Sources/main_c.obj" \
"./Sources/sdcard_c.obj" \
"./Sources/supersonic_c.obj" \
"./Sources/time_c.obj" \
"./Sources/video_c.obj" \

C_DEPS += \
./Sources/Exceptions_c.d \
./Sources/IntcInterrupts_c.d \
./Sources/L3G4200D_c.d \
./Sources/RFIDCard_c.d \
./Sources/UART_c.d \
./Sources/WiFi_c.d \
./Sources/action_c.d \
./Sources/analyse_c.d \
./Sources/camcontrol_c.d \
./Sources/control_c.d \
./Sources/diskio_c.d \
./Sources/drifting_c.d \
./Sources/ff_c.d \
./Sources/init_c.d \
./Sources/ivor_branch_table_c.d \
./Sources/lcd_c.d \
./Sources/mag_c.d \
./Sources/main_c.d \
./Sources/sdcard_c.d \
./Sources/supersonic_c.d \
./Sources/time_c.d \
./Sources/video_c.d \

OBJS_OS_FORMAT += \
./Sources/Exceptions_c.obj \
./Sources/IntcInterrupts_c.obj \
./Sources/L3G4200D_c.obj \
./Sources/RFIDCard_c.obj \
./Sources/UART_c.obj \
./Sources/WiFi_c.obj \
./Sources/action_c.obj \
./Sources/analyse_c.obj \
./Sources/camcontrol_c.obj \
./Sources/control_c.obj \
./Sources/diskio_c.obj \
./Sources/drifting_c.obj \
./Sources/ff_c.obj \
./Sources/init_c.obj \
./Sources/ivor_branch_table_c.obj \
./Sources/lcd_c.obj \
./Sources/mag_c.obj \
./Sources/main_c.obj \
./Sources/sdcard_c.obj \
./Sources/supersonic_c.obj \
./Sources/time_c.obj \
./Sources/video_c.obj \

C_DEPS_QUOTED += \
"./Sources/Exceptions_c.d" \
"./Sources/IntcInterrupts_c.d" \
"./Sources/L3G4200D_c.d" \
"./Sources/RFIDCard_c.d" \
"./Sources/UART_c.d" \
"./Sources/WiFi_c.d" \
"./Sources/action_c.d" \
"./Sources/analyse_c.d" \
"./Sources/camcontrol_c.d" \
"./Sources/control_c.d" \
"./Sources/diskio_c.d" \
"./Sources/drifting_c.d" \
"./Sources/ff_c.d" \
"./Sources/init_c.d" \
"./Sources/ivor_branch_table_c.d" \
"./Sources/lcd_c.d" \
"./Sources/mag_c.d" \
"./Sources/main_c.d" \
"./Sources/sdcard_c.d" \
"./Sources/supersonic_c.d" \
"./Sources/time_c.d" \
"./Sources/video_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Exceptions_c.obj: ../Sources/Exceptions.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Exceptions.args" -o "Sources/Exceptions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/IntcInterrupts_c.obj: ../Sources/IntcInterrupts.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/IntcInterrupts.args" -o "Sources/IntcInterrupts_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/L3G4200D_c.obj: ../Sources/L3G4200D.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/L3G4200D.args" -o "Sources/L3G4200D_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/RFIDCard_c.obj: ../Sources/RFIDCard.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/RFIDCard.args" -o "Sources/RFIDCard_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART_c.obj: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/UART.args" -o "Sources/UART_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/WiFi_c.obj: ../Sources/WiFi.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/WiFi.args" -o "Sources/WiFi_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/action_c.obj: ../Sources/action.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/action.args" -o "Sources/action_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/analyse_c.obj: ../Sources/analyse.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/analyse.args" -o "Sources/analyse_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/camcontrol_c.obj: ../Sources/camcontrol.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/camcontrol.args" -o "Sources/camcontrol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/control_c.obj: ../Sources/control.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/control.args" -o "Sources/control_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/diskio_c.obj: ../Sources/diskio.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/diskio.args" -o "Sources/diskio_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/drifting_c.obj: ../Sources/drifting.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/drifting.args" -o "Sources/drifting_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/ff_c.obj: ../Sources/ff.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/ff.args" -o "Sources/ff_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/init_c.obj: ../Sources/init.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/init.args" -o "Sources/init_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/ivor_branch_table_c.obj: ../Sources/ivor_branch_table.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/ivor_branch_table.args" -o "Sources/ivor_branch_table_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_c.obj: ../Sources/lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/lcd.args" -o "Sources/lcd_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/mag_c.obj: ../Sources/mag.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/mag.args" -o "Sources/mag_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/sdcard_c.obj: ../Sources/sdcard.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/sdcard.args" -o "Sources/sdcard_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/supersonic_c.obj: ../Sources/supersonic.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/supersonic.args" -o "Sources/supersonic_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/time_c.obj: ../Sources/time.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/time.args" -o "Sources/time_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/video_c.obj: ../Sources/video.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/video.args" -o "Sources/video_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


