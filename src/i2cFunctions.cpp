//
// Created by filip on 28.08.2022.
//

#include "i2cFunctions.h"

std::vector<uint> i2c_fun::getConnectedDevices(i2c_inst_t *i2c) {
    std::vector<uint> results;
    int ret;
    uint8_t rxdata;

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);

        if (ret >= 0) {
            results.push_back(addr);
        }
    }
    return std::move(results);
}

std::string i2c_fun::getNameOfModule(uint address) {
    for (auto &addr: MODULES_LIST) {
        if (addr == address) {
            return std::move(string(MODULES_NAME_LIST.at(addr)));
        }
    }
    return std::move(string("Unknown!"));
}

I_Module *i2c_fun::getModule(uint addr, DashboardMain *dBoard) {
    if (addr == 0x21)
        return new UniversalBusDesk();

    if (addr == 0x48)
        return new Adjuster();

    return nullptr;
}
