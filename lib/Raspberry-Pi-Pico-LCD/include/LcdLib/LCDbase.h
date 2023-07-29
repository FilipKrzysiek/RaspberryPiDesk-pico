//
// Created by filip on 12.02.2021.
//

#ifndef PICO_LCD_LIB_LCDBASE_H
#define PICO_LCD_LIB_LCDBASE_H

#ifndef USE_4_BIT_DATA_BUS
#define USE_4_BIT_DATA_BUS 1
#endif //USE_4_BIT_DATA_BUS

#ifndef Use_I2C
#define Use_I2C 0
#else
#define USE_4_BIT_DATA_BUS 1
#endif //Use_I2C

// Define commands and values
#define LCD_CMD_CLEAR_SCREEN 0x01
#define LCD_CMD_CURSOR_RETURN 0x02
#define LCD_CMD_CURSOR_PROPERTIES 0x04
#define LCD_CMD_DISPLAY_CONTROL 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM 0x40
#define LCD_CMD_SET_DDRAM 0x80

// flags for LCD_CMD_CURSOR_PROPERTIES
#define LCD_FLG_CURSOR_PROPERTIES_DECREMENT 0x00
#define LCD_FLG_CURSOR_PROPERTIES_INCREMENT 0x02
#define LCD_FLG_CURSOR_PROPERTIES_NOT_SHIFT 0x00
#define LCD_FLG_CURSOR_PROPERTIES_SHIFT 0x01

// flags for LCD_CMD_CURSOR_PROPERTIES
#define LCD_FLG_DISPLAY_CONTROL_DISPLAY_OFF 0x00
#define LCD_FLG_DISPLAY_CONTROL_DISPLAY_ON 0x04
#define LCD_FLG_DISPLAY_CONTROL_CURSOR_OFF 0x00
#define LCD_FLG_DISPLAY_CONTROL_CURSOR_ON 0x02
#define LCD_FLG_DISPLAY_CONTROL_BLINK_OFF 0x00
#define LCD_FLG_DISPLAY_CONTROL_BLINK_ON 0x01

//flags for LCD_CMD_CURSOR_SHIFT
#define LCD_FLG_CURSOR_SHIFT_CURSOR_SHIFT 0x00
#define LCD_FLG_CURSOR_SHIFT_DISPLAY_SHIFT 0x08
#define LCD_FLG_CURSOR_SHIFT_LEFT 0x00
#define LCD_FLG_CURSOR_SHIFT_RIGHT 0x04

//flags for LCD_CMD_FUNCTION_SET
#define LCD_FLG_CMD_FUNCTION_SET_1_LINE 0x00
#define LCD_FLG_CMD_FUNCTION_SET_2_LINE 0x08
#define LCD_FLG_CMD_FUNCTION_SET_FONT_5x7 0x00
#define LCD_FLG_CMD_FUNCTION_SET_FONT_5x10 0x04

#if USE_4_BIT_DATA_BUS != 1
#undef LCD_CMD_FUNCTION_SET
#define LCD_CMD_FUNCTION_SET 0x30
#endif //USE_4_BIT_DATA_BUS




#include "pico/stdlib.h"
#include "LcdHardwareInterface.h"

class LCDbase {
public:
    enum Direction {RIGHT, LEFT};
    enum State {ON, OFF};
    enum Font {STYLE_5x7, STYLE_5x10};

    LCDbase(LcdHardwareInterface *lcdHardwareAccess);

    virtual ~LCDbase();

    /**
     * Clear screen, delete anything, what was displayed on your screen
     */
    void clearScreen();

    /**
     * Move cursor to base point (line 0, place 0)
     */
    void cursorMoveToBasePoint();

    /**
     * WARNING this is working incorrect!! If its on, when you add new char, content of display was shifted in specified direction
     * @param state turn on, or off, use defined enum
     * @param direction direction fo move (default right), use defined enum
     */
    void autoShiftOnNewChar(State state = ON, Direction direction = RIGHT);

    /**
     * Enable/disable display
     * @param state turn on, or off, use defined enum
     */
    void display(State state = ON);

    /**
     * Enable/disable underline place of your cursor
     * @param state turn on, or off, use defined enum
     */
    void cursor(State state = ON);

    /**
     * Enable/disable blinking place of your cursor
     * @param state turn on, or off, use defined enum
     */
    void blink(State state = ON);

    /**
     * Move cursor
     * @param direction direction fo move (default right), use defined enum
     */
    void cursorMove(Direction direction = RIGHT);

    /**
     * Move content of your display on character to left or right
     * @param direction direction fo move (default right), use defined enum
     */
    void displayContentMove(Direction direction = RIGHT);

    /**
     * Initializing display
     * @param lineSize how many chars can you put to one line on your display
     * @param amountLines how many lines have your display
     * @param font You use font 5x7 (default), or 5x10, use defined enum
     */
    void initialize(unsigned short lineSize, unsigned short amountLines = 1, Font font = STYLE_5x7);

    /**
     * Move cursor to passed address
     * @param address number of place on screen
     */
    void cursorMoveTo(uint8_t address);

    /**
     * Defining new character. You can define 8 characters. IMPORTANT after define all new character must call sleep_ms(1001)
     * @param location number of character (0 - 8)
     * @param dots array with 8 rows representing new character
     */
    void defineChar(unsigned short location, const uint8_t *dots);

    /**
     * Write to screen passed character.
     * @param character that what you what to see on screen (remember of font table from your display specyfication)
     */
    void writeChar(uint8_t character);

    /**
     * Enable screen backlight (default enabled)
     */
    void enableBacklight();

    /**
     * Disable screen backlight (default enabled)
     */
    void disableBacklight();

protected:
    unsigned short amountLines;
    unsigned short lineSize;
    LcdHardwareInterface *hardwareAccess;

private:
    uint8_t displayControlState = 0;
};


#endif //PICO_LCD_LIB_LCDBASE_H
