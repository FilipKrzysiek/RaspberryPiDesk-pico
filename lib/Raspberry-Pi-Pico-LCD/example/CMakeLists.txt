cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(PicoLcdLibExample)

add_executable(PicoLcdLibExample "example/main.cpp")
target_link_libraries(PicoLcdLibExample pico_stdlib PicoLcdLib)

# enable usb output, disable uart output
pico_enable_stdio_usb(PicoLcdLibExample 1)
pico_enable_stdio_uart(PicoLcdLibExample 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(PicoLcdLibExample)