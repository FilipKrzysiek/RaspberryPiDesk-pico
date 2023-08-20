//
// Created by filip on 3/26/23.
//

#ifndef RASPBERRYPIDESK_PICO_DASHBOARDMENU_H
#define RASPBERRYPIDESK_PICO_DASHBOARDMENU_H


#include "DashboardMain.h"
#include <pico/stdlib.h>
#include <string>
#include <vector>

using namespace std;

class DashboardMenu {
    DashboardMain *dBoard;
    string title;

    uint8_t selectedIndex = 0;

    vector<string> labelList;

    void buttonActionCursorPosition(const unsigned &buttonStatus);

    void updateLabels();

    void updateLabels4Lines();

    void updateLabels3Lines();

    void drawMenu();

public:
    explicit DashboardMenu(DashboardMain *dBoard, const vector<string> &labelList);

    explicit DashboardMenu(DashboardMain *dBoard, const string &title, const vector<string> &labelList);

    void updateLabelsList(const vector<string> &labelList);

    void updateTitle(const string &newTitle);

    int updateMenu();

    inline void forceUpdateLabels() {
        updateLabels();
    }

};


#endif //RASPBERRYPIDESK_PICO_DASHBOARDMENU_H
