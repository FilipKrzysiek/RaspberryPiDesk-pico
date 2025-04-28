//
// Created by filip on 10.09.2022.
//

#ifndef RASPBERRYPIDESK_PICO_I_MODULE_HPP
#define RASPBERRYPIDESK_PICO_I_MODULE_HPP

#include <pico/stdlib.h>
#include "DashboardMain.h"
#include <bitset>

class DashboardMain;

class I_Module {
protected:
    const uint8_t addr;
    bool initialized = false;

public:
    const uint8_t D_SIZE_TD = 0; /**< Bytes send to device*/
    const uint8_t D_SIZE_FD = 0; /**< Bytes received from device*/
    virtual ~I_Module() = default;

    explicit I_Module(uint8_t addr, uint8_t bytesToDevice, uint8_t bytesFromDevice): addr(addr), D_SIZE_TD(bytesToDevice), D_SIZE_FD(bytesFromDevice) {}

    /**
     * Initialize i2c interface and configure module
     * @return true if successfully initialized false when problems
     */
    virtual bool initialize() = 0;

    /**
     * If possible send signal reset and reinitialize interface.
     * If can't send signal, only reinitialize interface.
     * @return true if successfully reset false when problems
     */
    virtual bool reset() = 0;

    /**
     * Transmit date with device
     * @param toDevice data to device with size declared in module as public variable D_SIZE_TD
     * @return true - its' all OK, flase - error on communication
     */
    virtual bool communicate(const uint8_t *toDevice) = 0;

    /**
     * Getter returning pointer to data received (in communication) from device
     * @return Pointer to data received from device. Size of data is declared in module as public variable D_SIZE_FD
     */
    virtual uint8_t *getDataFromDevice() = 0;

    /**
     * Single call, when device running in test usbMode. Printing on display in lines 1, 2 and 3 debug information.
     * Method is called every 100ms.
     * @param dBoard pointer to DashboardMain allowing to get ibis buttons state and printing on display
     * @return read data from module
     */
    virtual unsigned testMode(DashboardMain *dBoard) = 0;

    /**
     * Getting i2c module address.
     * @return module address
     */
    uint8_t getModuleAddress() {
        return addr;
    }

    /**
     * @brief Checking that module was initialized
     * @return true if module was initalized, false if not
     */
    bool isInitialized() {
        return initialized;
    }
};

#endif //RASPBERRYPIDESK_PICO_I_MODULE_HPP
