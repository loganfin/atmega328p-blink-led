# based on https://www.kuon.ch/post/2018-07-11-avr-cmake/
cmake_minimum_required(VERSION 3.21)
project(blink LANGUAGES C CXX ASM)

# for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## AVR Chip Configuration
# 8Mhz
set(F_CPU 8000000UL)
set(MCU atmega328p)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUD 9600)
# The programmer to use, read avrdude manual for list
set(PROG_TYPE arduino)
# Path to upload the file
set(DEV_PATH /dev/ttyACM0)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSS_COMPILING 1)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

# Pass defines to compiler
add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
)
# mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
    # requireed for avrdude
    -mmcu=${MCU} # MCU

    # optional
    -std=gnu99 # C99 standard
    -Os # optimize
    -Wall # enable warnings
)

file(GLOB SRC_FILES "src/*.c") # Load all files in src folder
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

# Rename the output to .elf as we will create multiple files
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME}.elf)

# Strip binary for upload
add_custom_target(strip ALL avr-strip ${CMAKE_PROJECT_NAME}.elf DEPENDS ${CMAKE_PROJECT_NAME})

# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex DEPENDS strip)
# Transform binary into hex file, this is the eeprom part (empty if you don't use eeprom static variables)
add_custom_target(eeprom avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.eep DEPENDS strip)

# Upload the firmware with avrdude
add_custom_target(upload avrdude -c ${PROG_TYPE} -p ${MCU} -U flash:w:${CMAKE_PROJECT_NAME}.hex -P ${DEV_PATH} DEPENDS hex)

# Upload the eeprom with avrdude
add_custom_target(upload_eeprom avrdude -c ${PROG_TYPE} -p ${MCU} -U eeprom:w:${CMAKE_PROJECT_NAME}.eep DEPENDS eeprom)

# Clean extra files
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_PROJECT_NAME}.hex;${CMAKE_PROJECT_NAME}.eeprom;${CMAKE_PROJECT_NAME}.lst")
