//
// Created by filip on 3/25/23.
//

#include <bsp/board.h>
#include "deskDeckMode.h"

#include <cmath>

#include "DashboardMenu.h"
#include "constFunctions.h"

DeskDeckMode::DeskDeckMode(DashboardMain *dBoard) : dBoard(dBoard) {
    mode = &DeskDeckMode::mainButtons;
}

usbConnectionMode_t DeskDeckMode::run() {
    dBoard->clearDisplay();

    while (usbMode == USB_DESK) {
        (*this.*mode)();
        sleep_ms(50);
    }
    return usbMode;
}

void DeskDeckMode::mainButtons() {
    checkConnected();
    dBoard->readButtons();
    auto buttonsStatus = dBoard->getButtonsIbisStatusInt();
    if (screenSleep && buttonsStatus != 0) {
        wakeUpScreen();
    }

    if (buttonsStatus == BASE_KYB_D) {
        mode = &DeskDeckMode::selectMode;
    } else if (buttonsStatus == BASE_KYB_A) {
        mode = &DeskDeckMode::insertingPredefinedText;
    } else if (buttonsStatus == BASE_KYB_B) {
        mode = &DeskDeckMode::inventorMode;
    } else if (buttonsStatus == BASE_KYB_C) {
        mode = &DeskDeckMode::omsiKeyboardMode;
    } else if (buttonsStatus == BASE_KYB_1) {
        deskDeckReport.setKeyPressed(HID_KEY_F13);
    } else if (buttonsStatus == BASE_KYB_2) {
        deskDeckReport.setKeyPressed(HID_KEY_F14);
    } else if (buttonsStatus == BASE_KYB_3) {
        deskDeckReport.setKeyPressed(HID_KEY_F15);
    } else if (buttonsStatus == BASE_KYB_4) {
        deskDeckReport.setKeyPressed(HID_KEY_F16);
    } else if (buttonsStatus == BASE_KYB_5) {
        deskDeckReport.setKeyPressed(HID_KEY_F17);
    } else if (buttonsStatus == BASE_KYB_6) {
        deskDeckReport.setKeyPressed(HID_KEY_F18);
    } else if (buttonsStatus == BASE_KYB_7) {
        deskDeckReport.setKeyPressed(HID_KEY_F19);
    } else if (buttonsStatus == BASE_KYB_8) {
        deskDeckReport.setKeyPressed(HID_KEY_F20);
    } else if (buttonsStatus == BASE_KYB_9) {
        deskDeckReport.setKeyPressed(HID_KEY_F21);
    } else if (buttonsStatus == BASE_KYB_0) {
        deskDeckReport.setKeyPressed(HID_KEY_F22);
    } else {
        deskDeckReport.clearReport();
    }
}

void DeskDeckMode::selectMode() {
    dBoard->writeOnDisplay("Chose usbMode:  0-\x7F", "1 - Serial", "2 - USB joystick", "3 - USB generic");
    unsigned short val;
    while (true) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();

        if (val == MODE_SERIAl) {
            usbMode = USB_SERIAL;
        } else if (val == MODE_JOYSTICK) {
            usbMode = USB_JOYSTICK;
        } else if (val == MODE_GENERIC) {
            usbMode = USB_GENERIC;
        } else if (val == MODE_TESTING) {
            usbMode = USB_TEST_MODE;
        } else if (val == 0b0010000000000000) {
            backToMainMode();
        } else {
            sleep_ms(10);
            continue;
        }
        break;
    }

    dBoard->clearDisplay();
}

void DeskDeckMode::insertingPredefinedText() {
    DashboardMenu menu(dBoard, "Wstaw predefiniowane teksty:", {"1 " + predefiniedTextDsc[0],
                                                                "2 " + predefiniedTextDsc[1],
                                                                "3 " + predefiniedTextDsc[2],
                                                                "4 " + predefiniedTextDsc[3],
                                                                "5 " + predefiniedTextDsc[4],
                                                                "6 " + predefiniedTextDsc[5],
                                                                "7 " + predefiniedTextDsc[6],
                                                                "8 " + predefiniedTextDsc[7],
                                                                "9 " + predefiniedTextDsc[8]});

    auto timer = board_millis();
    unsigned short val = 0;
    bool flgTitle1 = true;
    while (val == 0 || val == 8 || val == 128) {
        if (board_millis() - timer > 5000) {
            if (flgTitle1) {
                menu.updateTitle("Wybierz numer");
            } else {
                menu.updateTitle("Wstaw predefiniowane teksty:");
            }

            flgTitle1 = !flgTitle1;
            timer = board_millis();
        } else {
            menu.updateMenu();
        }

        val = dBoard->getButtonsIbisStatusInt();

        if (val == BASE_KYB_1) {
            writeString(predefiniedText[0]);
        } else if (val == BASE_KYB_2) {
            writeString(predefiniedText[1]);
        } else if (val == BASE_KYB_3) {
            writeString(predefiniedText[2]);
        } else if (val == BASE_KYB_4) {
            writeString(predefiniedText[3]);
        } else if (val == BASE_KYB_5) {
            writeString(predefiniedText[4]);
        } else if (val == BASE_KYB_6) {
            writeString(predefiniedText[5]);
        } else if (val == BASE_KYB_7) {
            writeString(predefiniedText[6]);
        } else if (val == BASE_KYB_8) {
            writeString(predefiniedText[7]);
        } else if (val == BASE_KYB_9) {
            writeString(predefiniedText[8]);
        }

        sleep_ms(50);
    }

    backToMainMode();
    dBoard->clearDisplay();
}

void DeskDeckMode::inventorMode() {
    dBoard->writeOnDisplay("Inventor mode");

    uint val = 0;
    while (val != BASE_KYB_D) {
        dBoard->readButtons();
        val = dBoard->getButtonsIbisStatusInt();

        inventorModeKeyActions(val);
        sleep_ms(50);
    }

    backToMainMode();
    dBoard->clearDisplay();
}

void DeskDeckMode::inventorModeKeyActions(const uint &val) {
    if (val == BASE_KYB_1) {
        deskDeckReport.setKeyPressed(HID_KEY_HOME);
    } else if (val == BASE_KYB_2) {
        deskDeckReport.setKeyPressed(HID_KEY_F14);
    } else if (val == BASE_KYB_3) {
        deskDeckReport.setKeyPressed(HID_KEY_F15);
    } else if (val == BASE_KYB_4) {
        deskDeckReport.setKeyPressed(HID_KEY_F16);
    } else if (val == BASE_KYB_5) {
        deskDeckReport.setKeyPressed(HID_KEY_F17);
    } else if (val == BASE_KYB_6) {
        deskDeckReport.setKeyPressed(HID_KEY_F18);
    } else if (val == BASE_KYB_7) {
        deskDeckReport.setKeyPressed(HID_KEY_F19);
    } else if (val == BASE_KYB_8) {
        deskDeckReport.setKeyPressed(HID_KEY_F20);
    } else if (val == BASE_KYB_9) {
        deskDeckReport.setKeyPressed(HID_KEY_F21);
    } else if (val == BASE_KYB_0) {
        deskDeckReport.setKeyPressed(HID_KEY_F22);
    } else {
        deskDeckReport.clearReport();
    }
}

void DeskDeckMode::omsiKeyboardMode() {
  dBoard->writeOnDisplay("OMSI keyboard mode");

  uint val = 0;
  while (val != BASE_KYB_D) {
    dBoard->readButtons();
    val = dBoard->getButtonsIbisStatusInt();

    uint8_t col = tu_log2(val) % 4;
    uint8_t row = tu_log2(val) / 4;

    if (val != 0) {
      if (col == 3) {
        if (val == BASE_KYB_A) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_DIVIDE);
        } else if (val == BASE_KYB_B) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_MULTIPLY);
        } else if (val == BASE_KYB_C) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_SUBTRACT);
        }
      } else if (row == 3) {
        if (val == BASE_KYB_0) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_0);
        } else if (val == BASE_KYB_STAR) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_DECIMAL);
        } else if (val == BASE_KYB_HASH) {
          deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_ENTER);
        }
      } else {
        deskDeckReport.setKeyPressedWithCtrl(HID_KEY_KEYPAD_1 + col + 3 * row);
      }
    } else {
      deskDeckReport.clearReport();
    }
    sleep_ms(50);
  }

  backToMainMode();
  dBoard->clearDisplay();
}
void DeskDeckMode::writeString(const string &text) {
    for (auto ch : text) {
        auto codes = getKeyCodeFromChar(ch);
        if (codes.first != 0) {
            deskDeckReport.setKeyPressedWithShift(codes.second);
        } else {
            deskDeckReport.setKeyPressed(codes.second);
        }
    }

    deskDeckReport.clearReport();
}

void DeskDeckMode::checkConnected() {
    if (tud_connected() && !tud_suspended()) {
        if (screenSleep) {
            wakeUpScreen();
        } else {
            sleepTimer = board_millis();
        }
    } else if ((!tud_connected() || tud_suspended()) && !screenSleep) {
        if (board_millis() - sleepTimer > 15000) {
            screenSleep = true;
            dBoard->disableDisplayBacklight();
        }
    }

}

void DeskDeckMode::wakeUpScreen() {
    screenSleep = false;
    dBoard->enableDisplayBacklight();
    sleepTimer = board_millis();
}

void DeskDeckMode::backToMainMode() {
    mode = &DeskDeckMode::mainButtons;
    sleepTimer = board_millis();
}
