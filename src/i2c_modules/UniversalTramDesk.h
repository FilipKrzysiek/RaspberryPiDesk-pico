//
// Created by Filip on 12.04.2025.
//

#ifndef UNIVERSALTRAMDESK_H
#define UNIVERSALTRAMDESK_H
#include "I_module.hpp"


class UniversalTramDesk: public I_Module {
    uint8_t data[2]{};
public:
    explicit UniversalTramDesk();

    bool initialize() override;

    bool reset() override;

    bool communicate(const uint8_t *toDevice) override;

    uint8_t *getDataFromDevice() override;

    unsigned int testMode(DashboardMain *dBoard) override;

};



#endif //UNIVERSALTRAMDESK_H
