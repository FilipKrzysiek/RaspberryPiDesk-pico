//
// Created by filip on 12.02.2021.
//

#include "LcdLib/LCDgeneric.h"



#if LCD_GENERIC_USE_STRING == 1

void LCDgeneric::writeLine(unsigned short lineNum, std::string text) {
    cursorMoveTo(getFirstAddressOfLine(lineNum));

    for (int i = 0; i < lineSize; ++i) {
        if (text.size() > i)
            writeChar(text[i]);
        else writeChar(' ');
    }
}
#else

void LCDgeneric::writeLine(unsigned short lineNum, char *text) {
    cursorMoveTo(getFirstAddressOfLine(lineNum));

    int i = 0;
    for (; i < lineSize; ++i) {
        if (text[i] != '\0') {
            writeChar(text[i]);
            break;
        }
    }

    for (; i < lineSize; ++i) {
        writeChar(' ');
    }
}

#endif

inline uint LCDgeneric::getFirstAddressOfLine(unsigned short lineNumber) {
    if (amountLines > 1) {
        if (lineNumber % 2 == 1) {
            return 0x40 | ((lineNumber / (amountLines / 2)) * lineSize);
        } else {
            return ((lineNumber / (amountLines / 2)) * lineSize);
        }
    } else
        return 0;
}

void LCDgeneric::clearLine(unsigned short lineNum) {
    unsigned short cursorPosition = getFirstAddressOfLine(lineNum);

    for(uint8_t i = 0; i < lineSize; ++i) {
        cursorMoveTo(cursorPosition);
        writeChar(' ');
    }



}

LCDgeneric::LCDgeneric(LcdHardwareInterface *lcdHardwareAccess) : LCDbase(lcdHardwareAccess) {}
