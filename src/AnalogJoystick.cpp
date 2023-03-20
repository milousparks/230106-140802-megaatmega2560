#include "AnalogJoystick.h"
AnalogJoystick::AnalogJoystick(int xPin, int yPin, int buttonPin) {
    this->xPin = xPin;
    this->yPin = yPin;
    this->buttonPin = buttonPin;
    
}
void AnalogJoystick::begin() {
        pinMode(xPin, INPUT);
        pinMode(yPin, INPUT);
        button.attach(buttonPin, INPUT_PULLUP);  
        this->button.setPressedState(LOW);
        this->button.interval(2);   
}

uint16_t AnalogJoystick::readX() {
    return analogRead(xPin);
}
uint16_t AnalogJoystick::readY() {
    return analogRead(yPin);
}
bool AnalogJoystick::readButton()
{}




