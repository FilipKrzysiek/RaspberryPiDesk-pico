cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
project(PicoLcdLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT DEFINED PICO_LCD_LIB_SEPARATE_HW_ACCESS)
    set(PICO_LCD_LIB_SEPARATE_HW_ACCESS)
endif ()

if (PICO_LCD_LIB_SEPARATE_HW_ACCESS EQUAL 1)
    add_library(PicoLcdLibGpio include/LcdLib/LcdHardwareInterface.h
            src/LcdHardwareGpio.cpp include/LcdLib/LcdHardwareGpio.h)
    target_link_libraries(PicoLcdLibGpio pico_stdlib)
    add_library(PicoLcdLibI2c include/LcdLib/LcdHardwareInterface.h
            src/LcdHardwareI2c.cpp include/LcdLib/LcdHardwareI2c.h)
    target_link_libraries(PicoLcdLibI2c pico_stdlib hardware_i2c)

    add_library(PicoLcdLib "src/LCDbase.cpp" "include/LcdLib/LCDbase.h"
            "src/LCDgeneric.cpp" "include/LcdLib/LCDgeneric.h"
            include/LcdLib/LcdHardwareInterface.h)

    target_link_libraries(PicoLcdLib pico_stdlib)
else ()
    add_library(PicoLcdLib "src/LCDbase.cpp" "include/LcdLib/LCDbase.h"
            "src/LCDgeneric.cpp" "include/LcdLib/LCDgeneric.h"
            include/LcdLib/LcdHardwareInterface.h
            src/LcdHardwareGpio.cpp include/LcdLib/LcdHardwareGpio.h
            src/LcdHardwareI2c.cpp include/LcdLib/LcdHardwareI2c.h)

    target_link_libraries(PicoLcdLib pico_stdlib hardware_i2c)
endif ()

