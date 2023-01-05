cmake_minimum_required(VERSION 3.2)

set(CMAKE_TOOLCHAIN_FILE "cmake/avr-gcc.toolchain.cmake")

project(blink_led LANGUAGES C CXX ASM)

include_directories(
    include/
)

add_definitions(-DF_CPU=16000000)

add_avr_executable(${PROJECT_NAME} "atmega328p"
    src/main.cpp
)
