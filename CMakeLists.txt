cmake_minimum_required(VERSION 3.2)

set(CMAKE_TOOLCHAIN_FILE "cmake/avr-gcc.toolchain.cmake")

install(CODE
    "execute_process(
        COMMAND sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:blink_led-atmega328p.hex
    )"
)

project(blink_led LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DF_CPU=16000000)

add_avr_executable(${PROJECT_NAME} "atmega328p"
    src/main.c
)
