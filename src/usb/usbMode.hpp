//
// Created by filip on 07.01.2022.
//

#ifndef OMSI2_USBMODE_H
#define OMSI2_USBMODE_H

#include <class/hid/hid_device.h>

constexpr uint32_t usbReportInterval_ms = 5;


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
        uint8_t keyCode[6] = {0};
        uint8_t prevKeyCode[6] = {0};
        uint8_t modifier = 0;


        enum ToSendStatus { NOT_READY, SEND, READY_TO_SEND };


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
                flgToSend = SEND;
            } else {
                if (flgToSend == SEND) {
                    tud_hid_keyboard_report(1, 0, nullptr);
                    flgToSend = NOT_READY;
                }
            }
        }
    };


    /**
     * Joystick data and functions to send a USB report
     */
    class Joystick {
    public:
        /**
         * Report data structure, which will send by USB
         */
        struct report_t {
            uint16_t ibisButtons = 2; /**< Buttons from IBIS, one button one bit */

            /**
             * Other buttons.
             *
             * Bit | Description              | Module
             * --- | ------------------------ | -----
             * 0   | Emergency brake          | Adjuster
             * 1   | Left indicator           | Universal Tram Desk
             * 2   | Disable indicator        | Universal Tram Desk
             * 3   | Right indicator          | Universal Tram Desk
             * 4   | Change switch to left    | Universal Tram Desk
             * 5   | Disable switch change    | Universal Tram Desk
             * 6   | Change switch to right   | Universal Tram Desk
             * 7   | Unlock open door         | Universal Tram Desk
             * 8   | Lock/close door          | Universal Tram Desk
             */
            uint16_t buttons = 0;

            uint8_t adjuster = 127; /**< Adjuster level */
            uint8_t fill = 0; /**< Fill in data structure so that its size is 64 bits */
            uint16_t fill1 = 0; /**< Fill in data structure so that its size is 64 bits */
        };

    private:
        report_t report;

    public:
        void sendGpadReport() const {
            tud_hid_report(1, &report, sizeof(report));
        }

        report_t *data() {
            return &report;
        }
    };
}


extern usb_reports::DeskDeck deskDeckReport;
extern usb_reports::Joystick joystickReport;

#endif //OMSI2_USBMODE_H
