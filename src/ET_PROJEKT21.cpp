#include "definitions.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <MultiDriver.h>
#include <SyncDriver.h>
#include <AnalogJoystick.h>
#include "Endstop.h"

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
DRV8825 *stepper_x_ptr = &stepper_x;
DRV8825 *stepper_y_ptr = &stepper_y;
AnalogJoystick *joystick_ptr = &joystick;
Endstop endstop(X_MIN_PIN, X_MAX_PIN, &stepper_x, &stepper_y);

unsigned long prevMillis;
void setup(void)
{
  stepper_x.begin(RPM_X, MICROSTEPS);
  stepper_y.begin(RPM_Y, MICROSTEPS);

  Serial.begin(9600); // Start serial communication at 9600 bps
  joystick.begin();   // Initialize the joystick
  endstop.begin();    // Initialize the endstop

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
  endstop.update();         // Polls the endstop
  // Serial.print("loop");
  uint16_t x = joystick.readX();
  uint16_t y = joystick.readY();

  // Serial.print("x=");
  // Serial.print(x);
  // Serial.print("\n");
  // Serial.print("y=");
  // Serial.print(y);
  // Serial.print("\n");
  // while (!endstop.endstop_xmin.pressed()||!endstop.endstop_xmax.pressed())
  // {
  //   stepper_x.move(1);
  // }

  // Kalibration
  if (joystick.button.released() && endstop.isCalibrated == false)
  {
    endstop.calibrate();
    joystick.button.update();
  }
  // if (endstop.isCalibrated== true)
  // {
  //    Serial.print("Calibrated");
  //   Serial.print(endstop.x_stepps);
  // }

  uint16_t stepps_rotated = 0;
  if (joystick.button.released() && endstop.isCalibrated == true)
  {
    uint32_t wicklung_pro_lage = SPULEN_Breite / FASER_DIAMETER;
    uint32_t x_stepps_pro_lage = SPULEN_Breite * (endstop.x_stepps / X_Lenght);
    uint32_t y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * wicklung_pro_lage;
    uint32_t y_stepps_per_x_stepps = y_stepps_pro_lage / x_stepps_pro_lage;
    while (stepps_rotated < x_stepps_pro_lage)
    {
      stepper_y.move(y_stepps_per_x_stepps);
      stepper_x.move(1);
      stepps_rotated++;
    }
  }

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

  // if (joystick.button.released())
  //{
  //   stepper_y.rotate(360 * WICKELACHSE_UEBERSETZUNG);
  //   if(stepper_y.currentPosition() == 0)
  //   stepper_x.rotate(360 * WICKELACHSE_UEBERSETZUNG);
  // }
}
