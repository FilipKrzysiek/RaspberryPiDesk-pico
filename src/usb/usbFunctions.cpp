//
// Created by filip on 06.01.2022.
//

#include "usbFunctions.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void usb_task() {
    uint32_t start_ms = board_millis();

    tusb_init();

    while (usbCMode != USB_NONE) {
        tud_task();

        if (board_millis() - start_ms < usbReportInterval_ms) {
            sleep_ms(1);
        } else {
            start_ms = board_millis();
            send_hid_report();
        }
    }
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

void send_hid_report() {
// skip if hid is not ready yet
    if (!tud_hid_ready()) return;

    if (usbCMode == USB_JOYSTICK) {
//        tud_hid_report(1, &report, sizeof(report));
    } else if (usbCMode == USB_DESK) {
        deskDeckReport.sendHidReport();
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request

uint16_t
tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    // TODO not Implemented
//    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
//    (void) instance;
}

#pragma clang diagnostic pop
