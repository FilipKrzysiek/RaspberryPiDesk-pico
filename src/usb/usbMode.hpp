//
// Created by filip on 07.01.2022.
//

#ifndef OMSI2_USBMODE_H
#define OMSI2_USBMODE_H

#include <class/hid/hid_device.h>

const uint32_t usbReportInterval_ms = 5;

typedef enum {
    USB_NONE,
    USB_SERIAL,
    USB_JOYSTICK,
    USB_GENERIC,
    USB_DESK,
    USB_TEST_MODE
} usbConnectionMode_t;

extern usbConnectionMode_t usbCMode;


namespace usb_reports {
    class DeskDeck {
        uint8_t keyCode[6] = { 0 };
        uint8_t prevKeyCode[6] = { 0 };
        uint8_t modifier = 0;

        enum ToSendStatus {NOT_READY, SENDED, READY_TO_SEND};

        ToSendStatus flgToSend = NOT_READY;

        inline void setKeyPressedBase(const uint8_t &keycode) {
            if (keycode != this->keyCode[0]) {
                this->keyCode[0] = keycode;
                modifier = 0;
                flgToSend = READY_TO_SEND;
            }
        }

    public:
        void setKeyPressed(uint8_t keycode) {
            setKeyPressedBase(keycode);
            sleep_ms(usbReportInterval_ms + 1);
        }

        void setKeyPressedWithShift(uint8_t keycode) {
            setKeyPressedBase(keycode);
            modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
            sleep_ms(usbReportInterval_ms + 1);
        }

        void setKeyPressedWithCtrl(uint8_t keycode) {
            setKeyPressedBase(keycode);
            modifier = KEYBOARD_MODIFIER_LEFTCTRL;
            sleep_ms(usbReportInterval_ms + 1);
        }

        void clearReport() {
            keyCode[0] = 0;
            modifier = 0;
        }

        void sendHidReport() {
            if (flgToSend == READY_TO_SEND && keyCode[0] != 0) {
                tud_hid_keyboard_report(1, modifier, keyCode);
                flgToSend = SENDED;
            } else {
                if (flgToSend == SENDED) {
                    tud_hid_keyboard_report(1, 0, nullptr);
                    flgToSend = NOT_READY;
                }
            }
        }
    };
}


extern usb_reports::DeskDeck deskDeckReport;

#endif //OMSI2_USBMODE_H
