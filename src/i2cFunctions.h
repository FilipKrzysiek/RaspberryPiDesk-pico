//
// Created by filip on 28.08.2022.
//

#ifndef RASPBERRYPIDESK_PICO_I2CFUNCTIONS_H
#define RASPBERRYPIDESK_PICO_I2CFUNCTIONS_H

#include <vector>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string>
#include "declarations.hpp"
#include "i2c_modules/UniversalBusDesk.hpp"

namespace i2c_fun {
    using namespace std;

    inline bool reserved_addr(uint8_t addr) {
        return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
    }

    [[nodiscard]] vector<uint> getConnectedDevices(i2c_inst_t *i2c);

    string getNameOfModule(uint address);

    I_Module *getModule(uint addr, DashboardMain *dBoard);
}

#endif //RASPBERRYPIDESK_PICO_I2CFUNCTIONS_H
