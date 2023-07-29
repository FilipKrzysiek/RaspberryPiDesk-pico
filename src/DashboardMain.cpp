//
// Created by filip on 28.08.2022.
//

#include "DashboardMain.h"

DashboardMain::DashboardMain() {
    pullDownSlaves();

    lcd.initialize(20, 4, LCDbase::STYLE_5x10);
}

void DashboardMain::pullDownSlaves() {
    for (auto &pin: GP_KYB_SLAVE) {
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_down(pin);
    }
}

const bool *DashboardMain::getButtonsIbisStatus() const {
    return buttonsIbisStatus;
}

unsigned DashboardMain::getButtonsIbisStatusInt() const {
    unsigned val = 0;
    for (int i = 0; i < 16; ++i) {
        val = val | (buttonsIbisStatus[i] << i);
    }
    return val;
}

void DashboardMain::readButtons() {
    readKybButtons();
}

inline void DashboardMain::readKybButtons() {
    pullDownSlaves();
    pullUpKybMaster0();
    sleep_us(1);

    unsigned short i = 12;
    for (auto &btn: GP_KYB_SLAVE) {
        buttonsIbisStatus[i] = gpio_get(btn);
        ++i;
    }

    pullUpKybMaster1();
    i = 4;
    sleep_us(1);

    for (auto &btn: GP_KYB_SLAVE) {
        buttonsIbisStatus[i] = gpio_get(btn);
        ++i;
    }


    pullDownKybMasters();
    i = 0;

    for (auto &btn: GP_KYB_SLAVE) {
        gpio_set_dir(btn, GPIO_OUT);
        gpio_put(btn, true);
        sleep_us(1);
        buttonsIbisStatus[i + 8] = gpio_get(GP_KYB_MASTER_0);
        buttonsIbisStatus[i] = gpio_get(GP_KYB_MASTER_1);
        gpio_set_dir(btn, GPIO_IN);
        gpio_pull_down(btn);
        ++i;
    }

}

void DashboardMain::pullUpKybMaster0() {
    gpio_set_dir(GP_KYB_MASTER_1, GPIO_IN);
    gpio_pull_down(GP_KYB_MASTER_1);
    gpio_set_dir(GP_KYB_MASTER_0, GPIO_OUT);
    gpio_put(GP_KYB_MASTER_0, true);
}

void DashboardMain::pullUpKybMaster1() {
    gpio_set_dir(GP_KYB_MASTER_0, GPIO_IN);
    gpio_pull_down(GP_KYB_MASTER_0);
    gpio_set_dir(GP_KYB_MASTER_1, GPIO_OUT);
    gpio_put(GP_KYB_MASTER_1, true);
}

void DashboardMain::pullDownKybMasters() {
    gpio_set_dir(GP_KYB_MASTER_0, GPIO_IN);
    gpio_pull_down(GP_KYB_MASTER_0);
    gpio_set_dir(GP_KYB_MASTER_1, GPIO_IN);
    gpio_pull_down(GP_KYB_MASTER_1);
}

void DashboardMain::writeLine(const unsigned short lineNum, const std::string &text) {
//    static_assert(lineNum > 3, "DashboardMain::writeLine, passed line number grater than 3");
    //TODO static assert
    lcd.writeLine(lineNum, text);
}

void DashboardMain::clearDisplay() {
    lcd.clearScreen();
}

void DashboardMain::writeOnDisplay(const std::string &line0, const std::string &line1, const std::string &line2,
                                   const std::string &line3) {
    lcd.writeLine(0, line0);
    lcd.writeLine(1, line1);
    lcd.writeLine(2, line2);
    lcd.writeLine(3, line3);
}

void DashboardMain::enableDisplayBacklight() {
    lcd.enableBacklight();
}

void DashboardMain::disableDisplayBacklight() {
    lcd.disableBacklight();
}
