//
// Created by filip on 12.02.2021.
//

#ifndef BUTTON_FIRST_LCDGENERIC_H
#define BUTTON_FIRST_LCDGENERIC_H

#ifndef LCD_GENERIC_USE_STRING
#define LCD_GENERIC_USE_STRING 1
#endif

#ifndef LCD_USE_I2C
#define LCD_USE_I2C 0
#endif

#if LCD_GENERIC_USE_STRING == 1
#include <string>
#endif

#include "LCDbase.h"


class LCDgeneric: public LCDbase {
public:

    LCDgeneric(LcdHardwareInterface *lcdHardwareAccess);
    //TODO set one first char line
    //TODO loading showing boxes

#if LCD_GENERIC_USE_STRING == 1
    void writeLine(unsigned short lineNum, std::string text);

    //TODO scrolling line
    //void writeLineScroling();
#else
    void writeLine(unsigned short lineNum, char text[]);
#endif

    void clearLine(unsigned short lineNum);

    //void loading(unsigned short lineNum, );

private:
    inline uint getFirstAddressOfLine(unsigned short lineNumber);
};


#endif //BUTTON_FIRST_LCDGENERIC_H
