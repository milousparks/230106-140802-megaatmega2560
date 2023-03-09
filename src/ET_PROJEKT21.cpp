#include "definitions.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <MultiDriver.h>
#include <SyncDriver.h>
#include <AnalogJoystick.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#include "DRV8825.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/23 /* A4 */, /* data=*/17 /* A2 */, /* CS=*/16 /* A3 */, /* reset=*/U8X8_PIN_NONE);

/*Joystick*/

/* Klassen Def*/
DRV8825 stepper_x(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
DRV8825 stepper_y(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
AnalogJoystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);

unsigned long prevMillis;
void setup(void)
{
  stepper_x.begin(RPM_X, MICROSTEPS);
  stepper_y.begin(RPM_Y, MICROSTEPS);

  Serial.begin(9600);
  joystick.begin();

  pinMode(encoder0Btn, INPUT_PULLUP);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);

  // FULL GRAPHIC SMART CONTROLLER BY: MWENE ADOLFO
  u8g2.begin(/* menu_select_pin= */ BUTTON_DIO, /* menu_next_pin= */ ROT_EN_B, /* menu_prev_pin= */ ROT_EN_A, /* menu_home_pin= */ KILL_PIN);
  u8g2.setFont(u8g2_font_6x12_tr);
}

void loop(void)
{
  joystick.button.update(); // Polls the button
  // Serial.print("loop");
  uint16_t x = joystick.readX();
  uint16_t y = joystick.readY();

  //Serial.print("x=");
  //Serial.print(x);
  //Serial.print("\n");
  //Serial.print("y=");
  //Serial.print(y);
  //Serial.print("\n");

  if (x > 600)
  {
    stepper_y.rotate(8);
    stepper_x.rotate(2);
    // Serial.print("x+");
  }
  else if (x < 400)
  {
    stepper_x.rotate(-2);
    stepper_y.rotate(8);
    // Serial.print("x-");
  }
  else if (y > 600)
  {
    stepper_y.rotate(-2);
  }
  else if (y < 400)
  {
    stepper_y.rotate(2);
  };

  
  
  if (joystick.button.released())
  {
    stepper_y.rotate(360 * WICKELACHSE_UEBERSETZUNG);
    if(stepper_y.currentPosition() == 0
    stepper_x.rotate(360 * WICKELACHSE_UEBERSETZUNG);
  }
}
