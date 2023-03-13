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
  x_stepps = 59247; // Gemessene Stepps
  isCalibrated = false;
  this->wicklung_pro_lage = SPULEN_Breite / FASER_DIAMETER;
  this->x_stepps_pro_lage = SPULEN_Breite * (this->x_stepps / X_Lenght);
  this->y_steps_per_rotation= MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS;
  this->y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * this->wicklung_pro_lage;
  this->y_stepps_per_x_stepps = y_stepps_pro_lage / x_stepps_pro_lage;

  Serial.print("\n");
  Serial.print("Y stepps peer x stepps");
  Serial.print(this->y_stepps_per_x_stepps);
  Serial.print("\n");
  Serial.print("X stepps pro Lage");
  Serial.print("\n");
  Serial.print(this->x_stepps_pro_lage);
  Serial.print("\n");
  Serial.print("Y stepps pro Lage");
  Serial.print("\n");
  Serial.print(this->y_stepps_pro_lage);
  Serial.print("\n");
  Serial.print("Wicklungen pro lage");
  Serial.print("\n");
  Serial.print(this->wicklung_pro_lage);
  Serial.print("\n");
  Serial.print("Y stepps per rotation");
  Serial.print("\n");
  Serial.print(this->y_steps_per_rotation);

}
void Endstop::update()
{
  this->endstop_xmin.update();
  this->endstop_xmax.update();
}

void Endstop::calibrate()
{ 
  this->x_stepps=0;

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
  y_stepps_pro_lage=0;
  this->x_stepps_pro_lage = SPULEN_Breite * (this->x_stepps / X_Lenght);
  this->y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * this->wicklung_pro_lage;
  this->y_stepps_per_x_stepps = this->y_stepps_pro_lage /this->x_stepps_pro_lage;
  this->isCalibrated = true;
  //Serial.print("\n");
  //Serial.print("Y stepps peer x stepps");
  //Serial.print(this->y_stepps_per_x_stepps);
  //Serial.print("\n");
  //Serial.print("X stepps pro Lage");
  //Serial.print("\n");
  //Serial.print(this->x_stepps_pro_lage);
  //Serial.print("\n");
  //Serial.print("Y stepps pro Lage");
  //Serial.print("\n");
  //Serial.print(this->y_stepps_pro_lage);
  //Serial.print("\n");
  //Serial.print("Wicklungen pro lage");
  //Serial.print("\n");
  Serial.print(this->wicklung_pro_lage);
  
}
