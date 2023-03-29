//
// Created by filip on 28.08.2022.
//

#include "pico/multicore.h"
#include "mainFunctions.h"
#include "usb/usbMode.hpp"
#include "usb/usbFunctions.h"
#include "modes/testMode/TestMode.h"
#include "modes/deskDeck/deskDeckMode.h"

usbConnectionMode_t usbCMode = USB_NONE;
usb_reports::DeskDeck deskDeckReport;

int main() {
    auto *dBoard = initialize();
    scanAndShowModules(dBoard);
    sleep_ms(2500);

    {
        usbCMode = USB_DESK;
        multicore_launch_core1(usb_task);
        DeskDeckMode ddMode(dBoard);
        usbCMode = ddMode.run();
    }

    board_led_write(false);

    if (usbCMode == USB_SERIAL) {

        dBoard->writeOnDisplay("NOT IMPLEMENTED!");
    } else if (usbCMode == USB_JOYSTICK) {

        dBoard->writeOnDisplay("NOT IMPLEMENTED!");
    } else if (usbCMode == USB_GENERIC) {

        dBoard->writeOnDisplay("NOT IMPLEMENTED!");
    } else if (usbCMode == USB_TEST_MODE) {
        TestMode testMode(dBoard);
    } else {
        dBoard->writeOnDisplay("ERROR!");
    }

    return 0;
}
