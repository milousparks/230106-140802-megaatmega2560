#ifndef Endstop_h
#define Endstop_h
#include "Arduino.h"
#include <Bounce2.h>
#include <DRV8825.h>
#include "AnalogJoystick.h"
#include <definitions.h>

class Endstop
{
public:
    Endstop(int pin_xmin, int pin_xmax, DRV8825 *stepper_x, DRV8825 *stepper_y);
    void begin();
    bool read();
    Bounce2::Button endstop_xmin;
    Bounce2::Button endstop_xmax;
    void update();
    uint32_t x_stepps;
    void calibrate();
    AnalogJoystick *joystick;
    DRV8825 *stepper_x;
    DRV8825 *stepper_y;
    bool isCalibrated;
    uint32_t wicklung_pro_lage;
    uint32_t x_stepps_pro_lage;
    uint32_t y_stepps_pro_lage;
    uint32_t y_stepps_per_x_stepps;
    uint32_t y_steps_per_rotation;

private:
    int pin_xmin, pin_xmax;
};

#endif