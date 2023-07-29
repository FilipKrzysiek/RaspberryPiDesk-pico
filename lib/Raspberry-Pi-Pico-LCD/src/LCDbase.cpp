//
// Created by filip on 12.02.2021.
//

#include "LcdLib/LCDbase.h"

LCDbase::LCDbase(LcdHardwareInterface *lcdHardwareAccess) : hardwareAccess(lcdHardwareAccess) {};

LCDbase::~LCDbase() {
    delete hardwareAccess;
}

void LCDbase::clearScreen() {
    hardwareAccess->writeData(false, false, LCD_CMD_CLEAR_SCREEN);
    sleep_ms(2);
}

void LCDbase::cursorMoveToBasePoint() {
    hardwareAccess->writeData(false, false, LCD_CMD_CURSOR_RETURN);
    sleep_ms(2);
}

void LCDbase::autoShiftOnNewChar(LCDbase::State state, LCDbase::Direction direction) {
    hardwareAccess->writeData(false, false, LCD_CMD_CURSOR_PROPERTIES |
                                            (state == ON ? LCD_FLG_CURSOR_PROPERTIES_SHIFT
                                                         : LCD_FLG_CURSOR_PROPERTIES_NOT_SHIFT)
                                            | (direction == RIGHT ? LCD_FLG_CURSOR_PROPERTIES_INCREMENT
                                                                  : LCD_FLG_CURSOR_PROPERTIES_DECREMENT));
}

void LCDbase::display(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_DISPLAY_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_DISPLAY_OFF);
    hardwareAccess->writeData(false, false, LCD_CMD_DISPLAY_CONTROL | displayControlState);
}

void LCDbase::cursor(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_CURSOR_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_CURSOR_OFF);
    hardwareAccess->writeData(false, false, LCD_CMD_DISPLAY_CONTROL | displayControlState);
}

void LCDbase::blink(LCDbase::State state) {
    displayControlState = (state == ON ? displayControlState | LCD_FLG_DISPLAY_CONTROL_BLINK_ON :
                           displayControlState & LCD_FLG_DISPLAY_CONTROL_BLINK_OFF);
    hardwareAccess->writeData(false, false, LCD_CMD_DISPLAY_CONTROL | displayControlState);
}

void LCDbase::cursorMove(LCDbase::Direction direction) {
    hardwareAccess->writeData(false, false, LCD_CMD_CURSOR_SHIFT | LCD_FLG_CURSOR_SHIFT_CURSOR_SHIFT |
                                            (direction == RIGHT ? LCD_FLG_CURSOR_SHIFT_RIGHT
                                                                : LCD_FLG_CURSOR_SHIFT_LEFT));
}

void LCDbase::displayContentMove(LCDbase::Direction direction) {
    hardwareAccess->writeData(false, false, LCD_CMD_CURSOR_SHIFT | LCD_FLG_CURSOR_SHIFT_DISPLAY_SHIFT |
                                            (direction == RIGHT ? LCD_FLG_CURSOR_SHIFT_RIGHT
                                                                : LCD_FLG_CURSOR_SHIFT_LEFT));
}

void LCDbase::initialize(unsigned short lineSize, unsigned short amountLines, LCDbase::Font font) {
    this->lineSize = lineSize;
    this->amountLines = amountLines;

    sleep_ms(90);
#if USE_4_BIT_DATA_BUS == 1
    hardwareAccess->writeData4Bit(false, false, 0x3);

    sleep_ms(5);
    hardwareAccess->writeData(false, false, 0x3);

    sleep_ms(5);
    hardwareAccess->writeData(false, false, 0x3);

    sleep_us(200);
    hardwareAccess->writeData(false, false, 0x2);

#else
    hardwareAccess->writeData(false, false, 0x30);

    sleep_ms(5);
    hardwareAccess->writeData(false, false, 0x30);

    sleep_us(200);
    hardwareAccess->writeData(false, false, 0x30);

#endif

    hardwareAccess->writeData(false, false, LCD_CMD_FUNCTION_SET |
                                            (lineSize == 1 ? LCD_FLG_CMD_FUNCTION_SET_1_LINE
                                                           : LCD_FLG_CMD_FUNCTION_SET_2_LINE) |
                                            (font == STYLE_5x7 ? LCD_FLG_CMD_FUNCTION_SET_FONT_5x7
                                                               : LCD_FLG_CMD_FUNCTION_SET_FONT_5x10));

    display();
    clearScreen();
    autoShiftOnNewChar(OFF);

    sleep_ms(10);

}

void LCDbase::cursorMoveTo(uint8_t address) {
    hardwareAccess->writeData(false, false, LCD_CMD_SET_DDRAM | address);
}

void LCDbase::defineChar(unsigned short location, const uint8_t *dots) {
    if (location > 8) location = location % 8;
    hardwareAccess->writeData(false, false, LCD_CMD_SET_CGRAM | (location << 3));

    for (int i = 0; i < 8; ++i) {
        writeChar(dots[i]);
    }

    cursorMoveToBasePoint();
}

void LCDbase::writeChar(uint8_t character) {
    hardwareAccess->writeData(true, false, character);
}

void LCDbase::enableBacklight() {
    hardwareAccess->enableBackLight();
}

void LCDbase::disableBacklight() {
    hardwareAccess->disableBackLight();
}
