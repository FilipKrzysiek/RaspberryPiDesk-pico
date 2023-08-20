//
// Created by filip on 3/26/23.
//

#include "DashboardMenu.h"

DashboardMenu::DashboardMenu(DashboardMain *dBoard, const vector<string> &labelList) : dBoard(dBoard),
    labelList(labelList) {
    dBoard->clearDisplay();
    drawMenu();
}

DashboardMenu::DashboardMenu(DashboardMain *dBoard, const string &title, const vector<string> &labelList)
        : dBoard(dBoard), title(title), labelList(labelList) {
    dBoard->clearDisplay();
    drawMenu();
}

void DashboardMenu::updateLabelsList(const vector<string> &labelList) {
    this->labelList.clear();
    this->labelList = labelList;
    updateLabels();
}

void DashboardMenu::drawMenu() {
    if (!title.empty()) {
        dBoard->defineNewChar(CHAR_ADDR_AR_UP, CHAR_DOTS_AR_UP);
        dBoard->defineNewChar(CHAR_ADDR_AR_DOWN, CHAR_DOTS_AR_DOWN);
        dBoard->writeLine(0, title + " A\b B\1 D~");
    }

    updateLabels();
}

int DashboardMenu::updateMenu() {
    dBoard->readButtons();
    auto buttonStatus = dBoard->getButtonsIbisStatusInt();
    buttonActionCursorPosition(buttonStatus);

    if (buttonStatus == 32768) {
        return selectedIndex;
    }
    return -1;
}

void DashboardMenu::buttonActionCursorPosition(const unsigned &buttonStatus) {
    if (buttonStatus == 8 && selectedIndex > 0) {
        --selectedIndex;
        updateLabels();
    } else if (buttonStatus == 128 && selectedIndex < labelList.size() - 1) {
        ++selectedIndex;
        updateLabels();
    }
}

void DashboardMenu::updateLabels() {
    if (title.empty()) {
        updateLabels4Lines();
    } else {
        updateLabels3Lines();
    }
}

void DashboardMenu::updateTitle(const string &newTitle) {
    title = newTitle;
    drawMenu();
}

void DashboardMenu::updateLabels3Lines() {
    uint8_t base;
    if (selectedIndex < 1) {
        base = 0;
    } else if (selectedIndex > labelList.size() - 2) {
        base = std::max<uint>(labelList.size() - 3, 0);
    } else {
        base = selectedIndex - 1;
    }

    uint8_t i = 0;
    for (; i < std::min<uint>(3, labelList.size()); ++i) {
        if (base + i == selectedIndex) {
            dBoard->writeLine(i + 1, "~" + labelList[base + i]);
        } else {
            dBoard->writeLine(i + 1, labelList[base + i]);
        }
    }

    for (; i < 3; ++i) {
        dBoard->writeLine(i + 1, "");
    }
}

void DashboardMenu::updateLabels4Lines() {
    uint8_t base;
    if (selectedIndex < 1) {
        base = 0;
    } else if (selectedIndex > labelList.size() - 3) {
        base = std::max<uint>(labelList.size() - 4, 0);
    } else {
        base = selectedIndex - 1;
    }

    uint8_t i = 0;
    for (; i < std::min<uint>(4, labelList.size()); ++i) {
        if (base + i == selectedIndex) {
            dBoard->writeLine(i, "~" + labelList[base + i]);
        } else {
            dBoard->writeLine(i, labelList[base + i]);
        }
    }

    for (; i < 4; ++i) {
        dBoard->writeLine(i, "");
    }
}
