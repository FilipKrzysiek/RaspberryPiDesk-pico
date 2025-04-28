//
// Created by Filip on 01.01.2024.
//

#include "GamePadMode.h"

#include <bsp/board.h>

GamePadMode::GamePadMode(DashboardMain *d_board)
        : dBoard(d_board), inputStates(joystickReport.data()) {
    moduleAdjuster.initialize();
    moduleUniversalTramDesk.initialize();
}

void GamePadMode::getDataFromDBoard() {
    dBoard->readButtons();
    inputStates->ibisButtons = dBoard->getButtonsIbisStatusInt();
}

void GamePadMode::getDataFromAdjuster() {
    if (moduleAdjuster.isInitialized()) {
        if (!moduleAdjuster.communicate(nullptr)) {
            return;
        }

        auto *adjusterData = moduleAdjuster.getDataFromDevice();
        inputStates->adjuster = adjusterData[0];
        inputStates->buttons = (inputStates->buttons & 0b1111'1111'1111'1110) | (adjusterData[1] & 0b0000'0000'0000'0001);
    } else {
        if (flgTryToInitialize) {
            moduleAdjuster.initialize();
        }
    }
}

void GamePadMode::getDataFromTramDashboard() {
    if (moduleUniversalTramDesk.isInitialized()) {
        if (!moduleUniversalTramDesk.communicate(nullptr)) {
            return;
        }

        auto *tramData = moduleUniversalTramDesk.getDataFromDevice();
        inputStates->buttons = (inputStates->buttons & 0b0000'0000'0000'0001) | ((tramData[0] << 1) & 0b0000'0001'1111'1110);

    } else {
        if (flgTryToInitialize) {
            moduleUniversalTramDesk.initialize();
        }
    }
}

void GamePadMode::run() {
    uint32_t start_ms = board_millis();
    uint8_t tryToInitializeCounter = 0;

    while (true) {
        if (tryToInitializeCounter >= 10) {
            flgTryToInitialize = true;
            tryToInitializeCounter = 0;

            // dBoard->writeLine(1, std::to_string(inputStates->adjuster));
            // dBoard->writeLine(2, std::to_string(inputStates->ibisButtons));
            // dBoard->writeLine(3, std::to_string(tud_hid_ready()));
        }
        getDataFromDBoard();
        getDataFromAdjuster();
        getDataFromTramDashboard();

        if (board_millis() - start_ms < 100) {
            sleep_ms(5);
        } else {
            start_ms = board_millis();
        }
        flgTryToInitialize = false;
        ++tryToInitializeCounter;
    }
}
