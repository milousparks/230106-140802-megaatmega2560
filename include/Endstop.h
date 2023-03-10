#ifndef Endstop_h
#define Endstop_h
#include "Arduino.h"
#include <Bounce2.h>
#include <DRV8825.h>
#include "AnalogJoystick.h"

class Endstop {
public:
    Endstop(int pin_xmin, int pin_xmax, DRV8825 *stepper_x, DRV8825 *stepper_y);
    void begin();
    bool read();
    Bounce2::Button  endstop_xmin;
    Bounce2::Button  endstop_xmax;
    void update();
    uint32_t x_stepps;
    void calibrate();
    AnalogJoystick* joystick;
    DRV8825* stepper_x;
    DRV8825* stepper_y;
    bool isCalibrated;
private:
    int pin_xmin, pin_xmax;
};

#endif