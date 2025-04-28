cmake_minimum_required(VERSION 3.16)

include(pico_sdk_import.cmake)
include(FetchContent)

project(RaspberryPiDesk-pico)

set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_subdirectory(lib)
add_subdirectory(modules)

include_directories(lib/Raspberry-Pi-Pico-LCD/include)

add_executable(RaspberryPiDesk-pico
        src/main.cpp
        src/DashboardMain.cpp src/DashboardMain.h
        src/usb/usb_descriptors.cpp
        src/usb/usbMode.hpp
        src/mainFunctions.h
        src/i2cFunctions.h src/i2cFunctions.cpp
        include/I_module.hpp
        src/modes/testMode/TestMode.cpp src/modes/testMode/TestMode.h
        src/i2c_modules/UniversalBusDesk.cpp src/i2c_modules/UniversalBusDesk.hpp
        src/modes/deskDeck/deskDeckMode.cpp src/modes/deskDeck/deskDeckMode.h
        src/usb/usbFunctions.cpp src/usb/usbFunctions.h src/DashboardMenu.cpp src/DashboardMenu.h
        src/i2c_modules/adjuster.cpp
        src/i2c_modules/adjuster.h
        src/modes/gamePadMode/GamePadMode.cpp
        src/modes/gamePadMode/GamePadMode.h
        src/constFunctions.h
        src/modes/deskDeck/declarations.h
        src/i2c_modules/UniversalTramDesk.cpp
        src/i2c_modules/UniversalTramDesk.h)

target_include_directories(RaspberryPiDesk-pico PUBLIC src include)

target_link_libraries(RaspberryPiDesk-pico PUBLIC pico_stdlib pico_multicore tinyusb_device tinyusb_board PicoLcdLibI2c PicoLcdLib)

pico_add_extra_outputs(RaspberryPiDesk-pico)

#if (UNIX)
#    message("Adding post build flashing to PICO")
#    add_custom_command(TARGET RaspberryPiDesk-pico POST_BUILD
#            COMMAND "sudo openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c \"program RaspberryPiDesk-pico.elf verify reset exit\""
#            COMMENT "Flashing to PICO")
#    install(TARGET RaspberryPiDesk-pico
#            CODE "sudo openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c \"program RaspberryPiDesk-pico.elf verify reset exit\"")
#endif (UNIX)