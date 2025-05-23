//
// Created by filip on 28.08.2022.
//

#ifndef RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H
#define RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H

#include <pico/stdlib.h>
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

    __attribute__((__deprecated__))
    void buttonsStateUintToTab();

    LCDgeneric lcd = LCDgeneric(new LcdHardwareI2c(LCD_I2C, LCD_I2C_ADR,
                                                   LCD_I2C_SCL, LCD_I2C_SDA));

    //TODO Think to change it to uint (memory optimalization)
    bool buttonsIbisStatus[16] = {false, false, false, false, false, false, false, false, false, false, false, false,
                                  false, false, false, false};

    uint16_t actualButtonStatus = 0;
    uint16_t prevButtonStatus = 0;

public:
    DashboardMain();


    /**
     * Update/read from buttons data.
     */
    void readButtons();

    /**
     * Get states of buttons
     * @return reference to buttons bool[16] array
     * @deprecated This function is deprecated, will be removed in future
     */
    __attribute__((__deprecated__))
    [[nodiscard]] const bool *getButtonsIbisStatus() const;

    /**
     * Get states of buttons
     * @return buttons status as unsigned int
     */
    [[nodiscard]] unsigned getButtonsIbisStatusInt() const;

    /**
     * \brief Get status of buttons only this which changed to high state.
     * \return buttons status (on bit one button)
     */
    [[nodiscard]] unsigned getButtonsIbisChanged() const;

    /**
     * Write line to display
     * @param lineNum number of lie indexed from 0
     * @param text text to write on display
     */
    void writeLine(unsigned short lineNum, const std::string &text);

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

    void enableDisplayBacklight();

    void disableDisplayBacklight();

    static int64_t buzzerStop(alarm_id_t id, void *user_data);
};


#endif //RASPBERRYPIDESK_PICO_DASHBOARDMAIN_H
