//
// Created by filip on 10.09.2022.
//

#ifndef RASPBERRYPIDESK_PICO_I_MODULE_HPP
#define RASPBERRYPIDESK_PICO_I_MODULE_HPP

#include "DashboardMain.h"
#include <bitset>
#include <ConfigStorage.h>

class DashboardMain;

class I_Module {
protected:
    const uint8_t addr;
    bool initialized = false;
    inline static ConfigStorage storage;

public:
    const uint8_t D_SIZE_TD = 0; /**< Bytes send to a device*/
    const uint8_t D_SIZE_FD = 0; /**< Bytes received from a device*/
    virtual ~I_Module() = default;

    explicit I_Module(uint8_t addr, uint8_t bytesToDevice, uint8_t bytesFromDevice): addr(addr), D_SIZE_TD(bytesToDevice), D_SIZE_FD(bytesFromDevice) {}

    /**
     * Initialize i2c interface and configure module
     * @return true if successfully initialized false when problems
     */
    virtual bool initialize() = 0;

    /**
     * If possible, send signal reset and reinitialize interface.
     * If can't send signal, only reinitialize interface.
     * @return true if successfully reset false when problems
     */
    virtual bool reset() = 0;

    /**
     * Transmit date with a device
     * @param toDevice data to a device with size declared in module as public variable D_SIZE_TD
     * @return true - it's all OK, false - error on communication
     */
    virtual bool communicate(const uint8_t *toDevice) = 0;

    /**
     * Getter returning pointer to data received (in communication) from device
     * @return Pointer to data received from a device. Size of data is declared in module as public variable D_SIZE_FD
     */
    virtual uint8_t *getDataFromDevice() = 0;

    /**
     * Single call, when a device running in test usbMode. Printing on display in lines 1, 2 and 3 debug information.
     * The method is called every 100 ms.
     * @param dBoard pointer to DashboardMain allowing to get ibis buttons state and printing on display
     * @return read data from module
     */
    virtual unsigned testMode(DashboardMain *dBoard) = 0;

    /**
     * Getting i2c module address.
     * @return module address
     */
    uint8_t getModuleAddress() const {
        return addr;
    }

    /**
     * @brief Checking that module was initialized
     * @return true if module was initialized, false if not
     */
    bool isInitialized() const {
        return initialized;
    }
};

#endif //RASPBERRYPIDESK_PICO_I_MODULE_HPP
