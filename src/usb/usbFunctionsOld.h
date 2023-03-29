//
// Created by filip on 06.01.2022.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#ifndef OMSI_USBFUNCTIONS_H
#define OMSI_USBFUNCTIONS_H

#include <string>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "Dashboard.h"
#include "usb/usbMode.hpp"

using namespace std;

struct gamepadReport {
    uint16_t buttons;
    uint16_t ibis;
};

void send_hid_gPad_report(const uint16_t &ibis, const uint16_t &buttons);

inline void initialize(Dashboard *dBoard) {
    dBoard->writeLine(0, "Inicjalizacja");
    dBoard->setLedStatus(true, true, true, true, true);
    sleep_ms(1000);
    dBoard->setLedStatus(false, false, false, false, false);
}

inline void clearDisplay(Dashboard *dBoard) {
    dBoard->writeLine(0, "");
    dBoard->writeLine(1, "");
    dBoard->writeLine(2, "");
    dBoard->writeLine(3, "");
}

inline unsigned short *selectMode(Dashboard *dBoard) {
    dBoard->writeLine(0, "Wybierz tryb:");
    dBoard->writeLine(1, "1 - Serial");
    dBoard->writeLine(2, "2 - USB joistick");
    dBoard->writeLine(3, "3 - USB generic");

    dBoard->readButtons();
    auto *val = new unsigned short;
    *val = dBoard->getButtonsIbisStatusInt();

    while (*val != 1 && *val != 2 && *val != 4 && *val != 8 && *val != 16) {
        dBoard->readButtons();
        *val = dBoard->getButtonsIbisStatusInt();
        sleep_ms(10);
    }

    clearDisplay(dBoard);

    return val;
}

inline void gamepadMode(Dashboard *dBoard) {
    tusb_init();
    clearDisplay(dBoard);
    dBoard->writeLine(0, "Gamepad mode");
    uint16_t ibisButtons, buttons;

    const uint32_t interval_ms = 5;
    static uint32_t start_ms = 0;

    while (true) {
        if (board_millis() - start_ms < interval_ms) {
            sleep_ms(2);
            continue;
        }
        start_ms += interval_ms;

        tud_task();

        dBoard->readButtons();
        ibisButtons = dBoard->getButtonsIbisStatusInt();
        buttons = dBoard->getButtonsStatusInt();

        // Remote wakeup
        if (tud_suspended()) {
            // Wake up host if we are in suspend mode
            // and REMOTE_WAKEUP feature is enabled by host
            tud_remote_wakeup();
        } else {
//             Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
            send_hid_gPad_report(ibisButtons, buttons);
        }
    }
}

inline void testMode(Dashboard *dBoard) {
    uint16_t ibisButtons, buttons;
    dBoard->writeLine(0, "Testing mode!");
    string buttonsLine, ibisLine;

    while (true) {
        dBoard->readButtons();
        ibisButtons = dBoard->getButtonsIbisStatusInt();
        buttons = dBoard->getButtonsStatusInt();

        buttonsLine = "";
        ibisLine = "";

        for (int i = 0; i < 16; ++i) {
            buttonsLine += (((buttons >> i) & 1) ? '1' : '0');
            ibisLine += (((ibisButtons >> i) & 1) ? '1' : '0');
        }

        dBoard->writeLine(1, buttonsLine);
        dBoard->writeLine(2, ibisLine);
    }
}

inline uint initAllGpio() {
    uint pinsMask = 0;
    pinsMask = pinsMask | (1 << GP_BTN_0_LED);
    pinsMask = pinsMask | (1 << GP_BTN_1_LED);
    pinsMask = pinsMask | (1 << GP_BTN_2_LED);
    pinsMask = pinsMask | (1 << GP_BTN_3_LED);
    pinsMask = pinsMask | (1 << GP_BTN_4_LED);
    pinsMask = pinsMask | (1 << GP_BTN_MASTER);
    pinsMask = pinsMask | (1 << GP_BTN_SLAVE_0);
    pinsMask = pinsMask | (1 << GP_BTN_SLAVE_1);
    pinsMask = pinsMask | (1 << GP_BTN_SLAVE_2);
    pinsMask = pinsMask | (1 << GP_BTN_SLAVE_3);
    pinsMask = pinsMask | (1 << GP_BTN_SLAVE_4);
    pinsMask = pinsMask | (1 << GP_KYB_MASTER_0);
    pinsMask = pinsMask | (1 << GP_KYB_MASTER_1);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_0);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_1);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_2);
    pinsMask = pinsMask | (1 << GP_KYB_SLAVE_3);
    gpio_init_mask(pinsMask);

    return pinsMask;
}

#endif //OMSI_USBFUNCTIONS_H

#pragma clang diagnostic pop