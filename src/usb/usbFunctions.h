//
// Created by filip on 3/25/23.
//

#ifndef RASPBERRYPIDESK_PICO_USBFUNCTIONS_H
#define RASPBERRYPIDESK_PICO_USBFUNCTIONS_H

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "usb/usbMode.hpp"

struct deskDeckReport {
    uint8_t keyCode;
};

void usb_task();

void send_hid_report();

#endif //RASPBERRYPIDESK_PICO_USBFUNCTIONS_H
