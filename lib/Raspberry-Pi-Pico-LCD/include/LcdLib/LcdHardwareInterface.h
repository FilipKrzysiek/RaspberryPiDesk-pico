//
// Created by filip on 24.09.2021.
//

#ifndef OMSI_LCDHARDWAREINTERFACE_H
#define OMSI_LCDHARDWAREINTERFACE_H


class LcdHardwareInterface {
protected:
    uint commandSleepTime = 600;
public:
    virtual void writeData(bool rs, bool rw, uint8_t data) = 0;
    virtual void writeData4Bit(bool rs, bool rw, uint8_t data) = 0;
    virtual void enableBackLight() = 0;
    virtual void disableBackLight() = 0;
     //TODO add read data

    void setCommandSleepTime(uint sleepTime) {
        commandSleepTime = sleepTime;
    }

    virtual ~LcdHardwareInterface() = default;

};


#endif //OMSI_LCDHARDWAREINTERFACE_H
