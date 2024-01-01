//
// Created by Filip on 01.01.2024.
//

#ifndef CONSTFUNCTIONS_H
#define CONSTFUNCTIONS_H
#include <cstdint>
#include <utility>
#include <class/hid/hid_device.h>

constexpr std::pair<uint8_t, uint8_t> getKeyCodeFromChar(char ch) {
    uint8_t const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };
    return {conv_table[ch][0] ? KEYBOARD_MODIFIER_LEFTSHIFT : 0, conv_table[ch][1]};
}

#endif //CONSTFUNCTIONS_H
