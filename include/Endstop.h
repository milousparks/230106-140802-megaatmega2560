#ifndef Endstop_h
#define Endstop_h
#include "Arduino.h"
#include <Bounce2.h>

class Endstop {
public:
    Endstop(int pin_xmin, int pin_xmax);
    void begin();
    bool read();
    Bounce2::Button  endstop_xmin;
    Bounce2::Button  endstop_xmax;
    void update();
private:
    int pin_xmin, pin_xmax;
};

#endif