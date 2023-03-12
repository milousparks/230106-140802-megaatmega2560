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
#include <GEM_u8g2.h>
#include <qdec.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#include "DRV8825.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/23 /* A4 */, /* data=*/17 /* A2 */, /* CS=*/16 /* A3 */, /* reset=*/U8X8_PIN_NONE);

/*Joystick*/
const int Rot_a = 31;
const int Rot_b = 33;
/* Klassen Def*/
DRV8825 stepper_x(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
DRV8825 stepper_y(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
AnalogJoystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Endstop endstop(X_MIN_PIN, X_MAX_PIN, &stepper_x, &stepper_y);
::SimpleHacks::QDecoder qdec(ROT_EN_B, ROT_EN_A, true);

// Create variables that will be editable through the menu and assign them initial values
int rotaryCount = 0;
boolean enablePrint = false;
// void rotary_data();
GEMItem menuItemInt("Rotary Count:", rotaryCount);
GEMItem menuItemBool("Enable print:", enablePrint);
void printData(); // Forward declaration
GEMItem menuItemButton("Wickeln", printData);
GEMPage menuPageMain("Main Menu");
GEMPage menuPageWickeln("Wickeln");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);
void setupMenu()
{
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  menuPageWickeln.addMenuItem(menuItemInt);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}
unsigned long prevMillis;
void setup(void)
{
  stepper_x.begin(RPM_X, MICROSTEPS);
  stepper_y.begin(RPM_Y, MICROSTEPS);
  menuItemInt.setReadonly();
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

  /*Rotarty Encoder*/
  qdec.begin();
  // FULL GRAPHIC SMART CONTROLLER BY: MWENE ADOLFO
  u8g2.begin(/* menu_select_pin= */ BUTTON_DIO, /* menu_next_pin= */ ROT_EN_B, /* menu_prev_pin= */ ROT_EN_A, /* menu_home_pin= */ KILL_PIN);
  u8g2.setFont(u8g2_font_6x12_tr);
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void loop(void)
{
  joystick.button.update();                            // Polls the button
  endstop.update();                                    // Polls the endstop
  ::SimpleHacks::QDECODER_EVENT event = qdec.update(); // Polls the rotary encoder

  // Serial.print("loop");
  uint16_t x = joystick.readX();
  uint16_t y = joystick.readY();

  // increment for clockwise, decrement for counter-clockwise
  if (event & ::SimpleHacks::QDECODER_EVENT_CW)
  {
    rotaryCount = rotaryCount + 1;
    Serial.print("change: ");
    Serial.println(rotaryCount);
    menu.registerKeyPress(GEM_KEY_DOWN);
    //menu.drawMenu();
  }
  else if (event & ::SimpleHacks::QDECODER_EVENT_CCW)
  {
    rotaryCount = rotaryCount - 1;
    Serial.print("change: ");
    Serial.println(rotaryCount);
    menu.registerKeyPress(GEM_KEY_UP);
  }

  // Kalibration
  if (joystick.button.released() && endstop.isCalibrated == false)
  {
    Serial.print("Calibrating");
    endstop.calibrate();
    joystick.button.update();
  }

  

  // // if (endstop.isCalibrated== true)
  // // {
  // //    Serial.print("Calibrated");
  // //   Serial.print(endstop.x_stepps);
  // // }
  //
  // uint16_t stepps_rotated = 0;
  // if (joystick.button.released() && endstop.isCalibrated == true)
  // {
  //   uint32_t wicklung_pro_lage = SPULEN_Breite / FASER_DIAMETER;
  //   uint32_t x_stepps_pro_lage = SPULEN_Breite * (endstop.x_stepps / X_Lenght);
  //   uint32_t y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * wicklung_pro_lage;
  //   uint32_t y_stepps_per_x_stepps = y_stepps_pro_lage / x_stepps_pro_lage;
  //   while (stepps_rotated < x_stepps_pro_lage)
  //   {
  //     stepper_y.move(y_stepps_per_x_stepps);
  //     stepper_x.move(1);
  //     stepps_rotated++;
  //   }
  // }
  //
  // if (x > 600)
  // {
  //   stepper_y.rotate(8);
  //   stepper_x.rotate(2);
  //   // Serial.print("x+");
  // }
  // else if (x < 400)
  // {
  //   stepper_x.rotate(-2);
  //   stepper_y.rotate(8);
  //   // Serial.print("x-");
  // }
  // else if (y > 600)
  // {
  //   stepper_y.rotate(-2);
  // }
  // else if (y < 400)
  // {
  //   stepper_y.rotate(2);
  // };
  //
  if (joystick.button.released())
    //{
    //   stepper_y.rotate(360 * WICKELACHSE_UEBERSETZUNG);
    //   if(stepper_y.currentPosition() == 0)
    //   stepper_x.rotate(360 * WICKELACHSE_UEBERSETZUNG);
    // }
    if (menu.readyForKey())
    {
      // ...detect key press using U8g2 library
      // and pass pressed button to menu
      menu.registerKeyPress(u8g2.getMenuEvent());
    }
}

void printData()
{
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint)
  {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(rotaryCount);
    menu.drawMenu();
  }
  else
  {
    Serial.println("Printing is disabled, sorry:(");
  }
}