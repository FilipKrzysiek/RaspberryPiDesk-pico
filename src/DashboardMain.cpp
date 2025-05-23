//
// Created by filip on 28.08.2022.
//

#include "DashboardMain.h"
#include "hardware/i2c.h"

int64_t DashboardMain::buzzerStop(alarm_id_t id, void *user_data) {
    gpio_put(GP_BUZZER, false);

    return 0;
}

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
void DashboardMain::buttonsStateUintToTab() {
    for (int i = 0; i < 16; ++i) {
        buttonsIbisStatus[i] = (actualButtonStatus << i) & 1;
    }
}

const bool *DashboardMain::getButtonsIbisStatus() const {
    return buttonsIbisStatus;
}

unsigned DashboardMain::getButtonsIbisStatusInt() const {
    return actualButtonStatus;
}

unsigned DashboardMain::getButtonsIbisChanged() const {
    return (prevButtonStatus ^ actualButtonStatus) & actualButtonStatus;
}

void DashboardMain::readButtons() {
    readKybButtons();
}

inline void DashboardMain::readKybButtons() {
    pullDownSlaves();
    pullUpKybMaster0();
    sleep_us(1);
    prevButtonStatus = actualButtonStatus;
    actualButtonStatus = 0;

    unsigned short i = 12;
    for (auto &btn: GP_KYB_SLAVE) {
        actualButtonStatus = actualButtonStatus | (gpio_get(btn) << i);
        ++i;
    }

    pullUpKybMaster1();
    i = 4;
    sleep_us(1);

    for (auto &btn: GP_KYB_SLAVE) {
        actualButtonStatus = actualButtonStatus | (gpio_get(btn) << i);
        ++i;
    }


    pullDownKybMasters();
    i = 0;

    for (auto &btn: GP_KYB_SLAVE) {
        gpio_set_dir(btn, GPIO_OUT);
        gpio_put(btn, true);
        sleep_us(1);
        actualButtonStatus = actualButtonStatus | (gpio_get(GP_KYB_MASTER_0) << (i + 8));
        actualButtonStatus = actualButtonStatus | (gpio_get(GP_KYB_MASTER_1) << i);
        gpio_set_dir(btn, GPIO_IN);
        gpio_pull_down(btn);
        ++i;
    }

    buttonsStateUintToTab();
    if (actualButtonStatus != 0 && prevButtonStatus != actualButtonStatus) {
        gpio_put(GP_BUZZER, true);
        add_alarm_in_ms(GP_BUZZER_TIME, buzzerStop, nullptr, true);
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
