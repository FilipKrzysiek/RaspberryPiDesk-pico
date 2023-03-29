//
// Created by filip on 28.08.2022.
//

#ifndef RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H
#define RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H

#include <pico/stdlib.h>
#include "hardware/i2c.h"
#include "declarations.hpp"
#include "LcdLib/LCDbase.h"
#include "LcdLib/LcdHardwareI2c.h"
#include "LcdLib/LCDgeneric.h"

class DashboardMain {
private:
    inline void readKybButtons();

    inline void pullUpKybMaster0();

    inline void pullUpKybMaster1();

    inline void pullDownKybMasters();

    void pullDownSlaves();

    LCDgeneric lcd = LCDgeneric(new LcdHardwareI2c(LCD_I2C, LCD_I2C_ADR,
                                                   LCD_I2C_SCL, LCD_I2C_SDA));

    //TODO Think to change it to uint (memory optimalization)
    bool buttonsIbisStatus[16] = {false, false, false, false, false, false, false, false, false, false, false, false,
                                  false, false, false, false};
public:
    DashboardMain();


    /**
     * Update/read from buttons data.
     */
    void readButtons();

    /**
     * Get states of buttons
     * @return reference to buttons bool[16] array
     */
    [[nodiscard]] const bool *getButtonsIbisStatus() const;

    /**
     * Get states of buttons
     * @return buttons status as unsigned int
     */
    [[nodiscard]] unsigned getButtonsIbisStatusInt() const;

    /**
     * Write line to display
     * @param lineNum number of lie indexed from 0
     * @param text text to write on display
     */
    void writeLine(const unsigned short lineNum, const std::string &text);

    /**
     * Write text to all lines display
     * @param line0
     * @param line1
     * @param line2
     * @param line3
     */
    void writeOnDisplay(const std::string &line0, const std::string &line1 = "", const std::string &line2 = "",
                        const std::string &line3 = "");

    /**
     * Remove text from all lines and leave it empty
     */
    void clearDisplay();

    inline void defineNewChar(unsigned short location, const uint8_t *dots) {
        lcd.defineChar(location, dots);
    }
};


#endif //RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H
