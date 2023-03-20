#ifndef SETTINGS_H
#define SETTINGS_H
#include <Arduino.h>
#include "definitions.h"
#pragma once

class Settings
{
public:
    Settings();
    uint8_t spule_breite;
    float faser_durchmesser;
    void begin();
    void calculateWindings();
    uint16_t xsteps;
    uint32_t wicklung_pro_lage;
    uint32_t x_stepps_pro_lage;
    uint32_t y_stepps_pro_lage;
    uint32_t y_stepps_per_x_stepps;
    uint32_t y_steps_per_rotation;

private:

};

#endif