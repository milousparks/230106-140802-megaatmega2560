#include "Endstop.h"

Endstop::Endstop(int pin_xmin, int pin_xmax,DRV8825 *stepper_x, DRV8825 *stepper_y) {
    this->pin_xmin = pin_xmin;
    this->pin_xmax = pin_xmax;
    this->stepper_x = stepper_x;
    this->stepper_y = stepper_y;
}

void Endstop::begin() {
    endstop_xmin.attach(pin_xmin, INPUT_PULLUP);
    endstop_xmin.interval(2);
    endstop_xmax.attach(pin_xmax, INPUT_PULLUP);
    endstop_xmax.interval(2);
    endstop_xmax.setPressedState(LOW);
    endstop_xmin.setPressedState(LOW);
    x_stepps=0;  
}
void Endstop::update() {
    this->endstop_xmin.update();
    this->endstop_xmax.update();
}

void Endstop::calibrate() {
    
  while (!this->endstop_xmax.pressed())
  {
    stepper_x->move(-1);
    Endstop::update();
  }
  stepper_x->move(-10);
  while (this->endstop_xmax.isPressed())
  {
    stepper_x->move(1);
    Endstop::update();
  }
  while (!endstop_xmin.pressed())
  {
    stepper_x->move(1);
    this->x_stepps++;
    Endstop::update();
  }
}

