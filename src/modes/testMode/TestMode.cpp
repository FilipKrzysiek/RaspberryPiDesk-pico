//
// Created by filip on 30.10.2022.
//

#include "TestMode.h"

[[noreturn]] TestMode::TestMode(DashboardMain *dBoard) : dBoard(dBoard) {
    dBoard->defineNewChar(CHAR_ADDR_AR_UP, CHAR_DOTS_AR_UP);
    dBoard->defineNewChar(CHAR_ADDR_AR_DOWN, CHAR_DOTS_AR_DOWN);
    dBoard->writeOnDisplay("Test mode! A\b B\1 D~");

    refreshConnectedModules();
    refreshDisplayConnected();

    while (true) {
        auto oldSize = connectedModules.size();

        refreshConnectedModules();

        flgRefreshDisplay = oldSize != connectedModules.size();

        dBoard->readButtons();
        readButtons = dBoard->getButtonsIbisStatusInt();
        buttonAction();
        if (flgRefreshDisplay) {
            refreshDisplayConnected();
            flgRefreshDisplay = false;
        }

        sleep_ms(100);
    }
}

void TestMode::buttonAction() {
    if (readButtons == 8 && selectedIndex > 0) {
        --selectedIndex;
        flgRefreshDisplay = true;
    } else if (readButtons == 128 && selectedIndex < connectedModules.size() - 1) {
        ++selectedIndex;
        flgRefreshDisplay = true;
    } else if (readButtons == 32768) {
        if (connectedModules[selectedIndex] == IBIS_BUTTONS_I2C_FANTOM) {
            ibisButtonsTM();
        } else {
            moduleTM();
        }
        //TODO run class
    }
}

void TestMode::refreshDisplayConnected() {
    uint8_t base;
    if (selectedIndex < 1) {
        base = 0;
    } else if (selectedIndex > connectedModules.size() - 2) {
        base = std::max<uint>(connectedModules.size() - 3, 0);
    } else {
        base = selectedIndex - 1;
    }

    uint8_t i = 0;
    for (; i < std::min<uint>(3, connectedModules.size()); ++i) {
        if (i == selectedIndex) {
            dBoard->writeLine(i + 1, "~" + getNameOfI2CModules(base + i));
        } else {
            dBoard->writeLine(i + 1, getNameOfI2CModules(base + i));
        }
    }

    for (; i < 3; ++i) {
        dBoard->writeLine(i + 1, "");
    }
}

std::string TestMode::getNameOfI2CModules(const uint &indx) {
    std::string text = "x";
    auto &addr = connectedModules[indx];
    if (addr < 16) {
        text += "0";
    } else {
        if (addr / 16 > 9)
            text += (addr / 16 - 10) + 'A';
        else
            text += (addr / 16) + '0';
    }

    if (addr % 16 > 9)
        text += (addr % 16 - 10) + 'A';
    else
        text += (addr % 16) + '0';

    text += ' ';

    text += i2c_fun::getNameOfModule(addr);

    return std::move(text);
}

[[noreturn]] void TestMode::ibisButtonsTM() {
    dBoard->writeOnDisplay("IBIS.md buttons Test Mode");
    while (true) {
        dBoard->readButtons();
        dBoard->writeLine(1, std::bitset<16>(dBoard->getButtonsIbisStatusInt()).to_string());

        sleep_ms(100);
    }
}

[[noreturn]] void TestMode::moduleTM() {
    I_Module *module = i2c_fun::getModule(connectedModules[selectedIndex], dBoard);
    if (module == nullptr || !module->initialize()) {
        dBoard->writeOnDisplay("ERROR ON CONNECTING!");
    } else {
        dBoard->writeOnDisplay("TM: " + std::string(MODULES_NAME_LIST.at(connectedModules[selectedIndex])));

        while(true) {
            module->testMode(dBoard);
            sleep_ms(100);
        }
    }
}

void TestMode::refreshConnectedModules() {
    connectedModules.clear();
    connectedModules = i2c_fun::getConnectedDevices(MODULES_I2C);
    connectedModules.push_back(IBIS_BUTTONS_I2C_FANTOM);
}
