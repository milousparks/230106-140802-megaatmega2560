#include "Endstop.h"


Endstop::Endstop(int pin_xmin, int pin_xmax, DRV8825 *stepper_x, DRV8825 *stepper_y)
{
  this->pin_xmin = pin_xmin;
  this->pin_xmax = pin_xmax;
  this->stepper_x = stepper_x;
  this->stepper_y = stepper_y;
}

void Endstop::begin()
{
  endstop_xmin.attach(pin_xmin, INPUT_PULLUP);
  endstop_xmin.interval(2);
  endstop_xmax.attach(pin_xmax, INPUT_PULLUP);
  endstop_xmax.interval(2);
  endstop_xmax.setPressedState(LOW);
  endstop_xmin.setPressedState(LOW);
  x_steps = X_STEPS; // Gemessene steps
  isCalibrated = false;
  this->wicklung_pro_lage = SPULEN_Breite / FASER_DIAMETER;
  this->x_steps_pro_lage = SPULEN_Breite * (this->x_steps / X_Lenght);
  this->y_steps_per_rotation= MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS;
  this->y_steps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * this->wicklung_pro_lage;
  this->y_steps_per_x_steps = y_steps_pro_lage / x_steps_pro_lage;

 // Serial.print("\n");
 // Serial.print("Y steps peer x steps");
 // Serial.print(this->y_steps_per_x_steps);
 // Serial.print("\n");
 // Serial.print("X steps pro Lage");
 // Serial.print("\n");
 // Serial.print(this->x_steps_pro_lage);
 // Serial.print("\n");
 // Serial.print("Y steps pro Lage");
 // Serial.print("\n");
 // Serial.print(this->y_steps_pro_lage);
 // Serial.print("\n");
 // Serial.print("Wicklungen pro lage");
 // Serial.print("\n");
 // Serial.print(this->wicklung_pro_lage);
 // Serial.print("\n");
 // Serial.print("Y steps per rotation");
 // Serial.print("\n");
 // Serial.print(this->y_steps_per_rotation);
//
}
void Endstop::update()
{
  this->endstop_xmin.update();
  this->endstop_xmax.update();
}

uint16_t Endstop::calibrate()
{ 
  this->x_steps=0;

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
    this->x_steps++;
    Endstop::update();
  }
  return x_steps;
  //y_steps_pro_lage=0;
  //this->x_steps_pro_lage = SPULEN_Breite * (this->x_steps / X_Lenght);
  //this->y_steps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * this->wicklung_pro_lage;
  //this->y_steps_per_x_steps = this->y_steps_pro_lage /this->x_steps_pro_lage;
  //this->isCalibrated = true;
  //Serial.print("\n");
  //Serial.print("Y steps peer x steps");
  //Serial.print(this->y_steps_per_x_steps);
  //Serial.print("\n");
  //Serial.print("X steps pro Lage");
  //Serial.print("\n");
  //Serial.print(this->x_steps_pro_lage);
  //Serial.print("\n");
  //Serial.print("Y steps pro Lage");
  //Serial.print("\n");
  //Serial.print(this->y_steps_pro_lage);
  //Serial.print("\n");
  //Serial.print("Wicklungen pro lage");
  //Serial.print("\n");
  // Serial.print(this->wicklung_pro_lage);
  
}
