#define USE_4_BIT_DATA_BUS 1

#define LCD_GENERIC_USE_STRING 1

#include "pico/stdlib.h"
#include "LcdLib/LCDbase.h"
#include "LcdLib/LCDgeneric.h"


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
 //   uint PIN_DATA[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint PIN_DATA[4] = {4, 5, 6, 7};
    uint PIN_INSTRUCTION_DATA = 8, PIN_READ_WRITE = 11, PIN_ENABLE = 9, PIN_LED = 25;
    uint8_t newChar[8] = {0b00000, 0b00000, 0b01010, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};
    uint8_t newChar1[8] = {0b00000, 0b00000, 0b01110, 0b10101, 0b11111, 0b10001, 0b01110, 0b00000};

    for(short i = 0; i < 4; ++i) {
        gpio_init(PIN_DATA[i]);
        gpio_set_dir(PIN_DATA[i], GPIO_OUT);
    }
    gpio_init(PIN_INSTRUCTION_DATA);
    gpio_set_dir(PIN_INSTRUCTION_DATA, GPIO_OUT);

    gpio_init(PIN_READ_WRITE);
    gpio_set_dir(PIN_READ_WRITE, GPIO_OUT);

    gpio_init(PIN_ENABLE);
    gpio_set_dir(PIN_ENABLE, GPIO_OUT);

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    gpio_put(PIN_LED, true);
    LCDgeneric lcd(PIN_READ_WRITE, PIN_INSTRUCTION_DATA, PIN_ENABLE, PIN_DATA);
    lcd.initialize(20, 4);

//    lcd.defineChar(0, newChar);
//    lcd.defineChar(1, newChar1);
    sleep_ms(1001);
    gpio_put(PIN_LED, false);

    sleep_ms(250);
    lcd.cursor();
    sleep_ms(1250);
    lcd.cursorMoveTo(40);
    //lcd.autoShiftOnNewChar(lcd.ON, lcd.RIGHT);


    lcd.writeChar('K');
    sleep_ms(250);
    lcd.writeChar('i');
    sleep_ms(250);
    lcd.writeChar('t');
    sleep_ms(250);
    lcd.writeChar('o');
    sleep_ms(250);
    lcd.writeChar(' ');
    sleep_ms(250);
    lcd.writeChar('p');
    sleep_ms(250);
    lcd.writeChar('i');
    sleep_ms(250);
    lcd.writeChar('t');
    sleep_ms(250);
    lcd.writeChar('o');
    sleep_ms(1250);

    lcd.writeLine(0, "Witam, Panstawa");
    sleep_ms(2250);
    lcd.writeLine(1, "Elo ziomale! Co tam u Was?");
    sleep_ms(2250);
    lcd.writeLine(2, "A w porzasiu! @ :)");
    sleep_ms(2250);
    lcd.writeLine(3, "Mniam, mniam mniam");

    lcd.blink();
    for( int i = 0; i < 60; ++i) {
        lcd.writeLine(0, std::to_string(i * 2));
        lcd.cursorMoveTo(i * 2);
        sleep_ms(2000);
    }


    while (true) {

    }
}

#pragma clang diagnostic pop