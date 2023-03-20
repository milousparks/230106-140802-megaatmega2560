#ifndef ANALOGJOYSTICK_H
#define ANALOGJOYSTICK_H
#include "Arduino.h"
#include <Bounce2.h>
class AnalogJoystick {
public:
    AnalogJoystick(int xPin, int yPin, int buttonPin);
    void begin();
    uint16_t readX();
    uint16_t readY();
    bool readButton();
    Bounce2::Button  button;
private:
    int xPin, yPin, buttonPin,buttonState, lastButtonState;
};
#endif // ANALOGJOYSTICK_H