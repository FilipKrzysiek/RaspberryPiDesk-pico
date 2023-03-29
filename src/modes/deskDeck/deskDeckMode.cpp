//
// Created by filip on 3/25/23.
//

#include <bsp/board.h>
#include "deskDeckMode.h"

DeskDeckMode::DeskDeckMode(DashboardMain *dBoard) : dBoard(dBoard) {
    mode = &DeskDeckMode::mainButtons;
}

usbConnectionMode_t DeskDeckMode::run() {
    dBoard->clearDisplay();

    while (usbMode == USB_DESK) {
        (*this.*mode)();
        sleep_ms(50);
    }
    return usbMode;
}

void DeskDeckMode::mainButtons() {
    dBoard->readButtons();
    auto buttonsStatus = dBoard->getButtonsIbisStatusInt();

    if (buttonsStatus == 0b1000000000000000) {
        mode = &DeskDeckMode::selectMode;
    } else if (buttonsStatus == 0b0000000000001000) {
        mode = &DeskDeckMode::insertingPredefinedText;
    } else if (buttonsStatus == 0b0000000010000000) {
        mode = &DeskDeckMode::inventorMode;
    } else if (buttonsStatus == 0b0000000000000001) {
        deskDeckReport.setKeyPressed(HID_KEY_F13);
    } else if (buttonsStatus == 0b0000000000000010) {
        deskDeckReport.setKeyPressed(HID_KEY_F14);
    } else if (buttonsStatus == 0b0000000000000100) {
        deskDeckReport.setKeyPressed(HID_KEY_F15);
    } else if (buttonsStatus == 0b0000000000010000) {
        deskDeckReport.setKeyPressed(HID_KEY_F16);
    } else if (buttonsStatus == 0b0000000000100000) {
        deskDeckReport.setKeyPressed(HID_KEY_F17);
    } else if (buttonsStatus == 0b0000000001000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F18);
    } else if (buttonsStatus == 0b0000000100000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F19);
    } else if (buttonsStatus == 0b0000001000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F20);
    } else if (buttonsStatus == 0b0000010000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F21);
    } else if (buttonsStatus == 0b0010000000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F22);
    } else {
        deskDeckReport.clearReport();
    }
}

void DeskDeckMode::selectMode() {
    dBoard->writeOnDisplay("Chose usbMode:  0-\x7F", "1 - Serial", "2 - USB joystick", "3 - USB generic");
    unsigned short val;
    while (true) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();

        if (val == MODE_SERIAl) {
            usbMode = USB_SERIAL;
        } else if (val == MODE_JOYSTICK) {
            usbMode = USB_JOYSTICK;
        } else if (val == MODE_GENERIC) {
            usbMode = USB_GENERIC;
        } else if (val == MODE_TESTING) {
            usbMode = USB_TEST_MODE;
        } else if (val == 0b0010000000000000) {
            mode = &DeskDeckMode::mainButtons;
        } else {
            sleep_ms(10);
            continue;
        }
        break;
    }

    dBoard->clearDisplay();
}

void DeskDeckMode::insertingPredefinedText() {
    dBoard->writeOnDisplay("Wstaw predefiniowane teksty:");

    unsigned short val = 0;
    while (val == 0) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();

        if (val == 0b0000000000000001) {
            writeString1();
        } else if (val == 0b0000000000000010) {
            writeString2();
        }

        sleep_ms(50);
    }

    mode = &DeskDeckMode::mainButtons;
    dBoard->clearDisplay();
}

void DeskDeckMode::writeString1() {
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressed(HID_KEY_R);
    deskDeckReport.setKeyPressed(HID_KEY_Z);
    deskDeckReport.setKeyPressed(HID_KEY_Y);
    deskDeckReport.setKeyPressed(HID_KEY_S);
    deskDeckReport.setKeyPressed(HID_KEY_Z);
    deskDeckReport.setKeyPressed(HID_KEY_T);
    deskDeckReport.setKeyPressed(HID_KEY_O);
    deskDeckReport.setKeyPressed(HID_KEY_F);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_S);
    deskDeckReport.setKeyPressed(HID_KEY_W);
    deskDeckReport.setKeyPressed(HID_KEY_A);
    deskDeckReport.setKeyPressed(HID_KEY_L);
    deskDeckReport.setKeyPressed(HID_KEY_D);
    deskDeckReport.setKeyPressed(HID_KEY_E);
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressedWithShift(HID_KEY_2);
    deskDeckReport.setKeyPressed(HID_KEY_S);
    deskDeckReport.setKeyPressed(HID_KEY_T);
    deskDeckReport.setKeyPressed(HID_KEY_U);
    deskDeckReport.setKeyPressed(HID_KEY_D);
    deskDeckReport.setKeyPressed(HID_KEY_E);
    deskDeckReport.setKeyPressed(HID_KEY_N);
    deskDeckReport.setKeyPressed(HID_KEY_T);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_P);
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_E);
    deskDeckReport.setKeyPressed(HID_KEY_D);
    deskDeckReport.setKeyPressed(HID_KEY_U);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_P);
    deskDeckReport.setKeyPressed(HID_KEY_L);
    deskDeckReport.clearReport();
}

void DeskDeckMode::writeString2() {
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressed(HID_KEY_R);
    deskDeckReport.setKeyPressed(HID_KEY_Z);
    deskDeckReport.setKeyPressed(HID_KEY_Y);
    deskDeckReport.setKeyPressed(HID_KEY_S);
    deskDeckReport.setKeyPressed(HID_KEY_Z);
    deskDeckReport.setKeyPressed(HID_KEY_T);
    deskDeckReport.setKeyPressed(HID_KEY_O);
    deskDeckReport.setKeyPressed(HID_KEY_F);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_S);
    deskDeckReport.setKeyPressed(HID_KEY_W);
    deskDeckReport.setKeyPressed(HID_KEY_A);
    deskDeckReport.setKeyPressed(HID_KEY_L);
    deskDeckReport.setKeyPressed(HID_KEY_D);
    deskDeckReport.setKeyPressed(HID_KEY_E);
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressedWithShift(HID_KEY_2);
    deskDeckReport.setKeyPressed(HID_KEY_P);
    deskDeckReport.setKeyPressed(HID_KEY_K);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_E);
    deskDeckReport.setKeyPressed(HID_KEY_D);
    deskDeckReport.setKeyPressed(HID_KEY_U);
    deskDeckReport.setKeyPressed(HID_KEY_PERIOD);
    deskDeckReport.setKeyPressed(HID_KEY_P);
    deskDeckReport.setKeyPressed(HID_KEY_L);
    deskDeckReport.clearReport();
}

void DeskDeckMode::inventorMode() {
    dBoard->writeOnDisplay("Inventor mode");

    uint val = 0;
    while (val != 8192) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();

        inventorModeKeyActions(val);
        sleep_ms(50);
    }

    mode = &DeskDeckMode::mainButtons;
    dBoard->clearDisplay();
}

void DeskDeckMode::inventorModeKeyActions(const uint &val) {
    if (val == 0b0000000000000001) {
        deskDeckReport.setKeyPressed(HID_KEY_HOME);
    } else if (val == 0b0000000000000010) {
        deskDeckReport.setKeyPressed(HID_KEY_F14);
    } else if (val == 0b0000000000000100) {
        deskDeckReport.setKeyPressed(HID_KEY_F15);
    } else if (val == 0b0000000000010000) {
        deskDeckReport.setKeyPressed(HID_KEY_F16);
    } else if (val == 0b0000000000100000) {
        deskDeckReport.setKeyPressed(HID_KEY_F17);
    } else if (val == 0b0000000001000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F18);
    } else if (val == 0b0000000100000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F19);
    } else if (val == 0b0000001000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F20);
    } else if (val == 0b0000010000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F21);
    } else if (val == 0b0010000000000000) {
        deskDeckReport.setKeyPressed(HID_KEY_F22);
    } else {
        deskDeckReport.clearReport();
    }
}
