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
int x;
uint16_t y;
/* Klassen Def*/
DRV8825 stepper_x(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
DRV8825 stepper_y(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
DRV8825 stepper_z(MOTOR_STEPS, Z_DIR_PIN, Z_STEP_PIN, Z_ENABLE_PIN);
AnalogJoystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Endstop endstop(X_MIN_PIN, X_MAX_PIN, &stepper_x, &stepper_y);
::SimpleHacks::QDecoder qdec(ROT_EN_B, ROT_EN_A, true);
Bounce2::Button rotary_button = Bounce2::Button();
float wicklungen;
// Create variables that will be editable through the menu and assign them initial values
int rotaryCount = 0;
boolean enablePrint = false;
// void rotary_data();
GEMItem menuItemInt("Rotary Count:", rotaryCount);
GEMItem menuItemBool("Enable print:", enablePrint);
void printData();       // Forward declaration
void wickelLageLeft();  // Forward declaration
void wickelLageRight(); // Forward declaration
void kalibrieren();     // Forward declaration
/*Menu Pages*/
GEMPage menuPageMain("Main Menu");
GEMPage menuPageWickeln("Wickeln");
GEMPage menuPageManWickeln("Manuell Wickeln");
GEMPage menuPageAutoWickeln("Automatisch Wickeln");

GEMItem menuItemWicklen("Wickeln", menuPageWickeln);
GEMItem menuItemManWicklen("Manuell Wickeln", menuPageManWickeln);
GEMItem menuItemAutoWicklen("Automatisch Wickeln", menuPageAutoWickeln);
GEMItem menuItemJoystickPosX("Joystick Pos. X:", x);
GEMItem menueItemAutoLageLeft("Wickel 1 Lage <-", wickelLageLeft);
GEMItem menueItemAutoLageRight("Wickel 1 Lage ->", wickelLageRight);
GEMItem menuItemWicklungen("Wicklungen:", wicklungen);	
GEMItem menuItemWickelnKalibrieren("Kalibrieren",kalibrieren);

uint32_t wicklung_pro_lage;
uint32_t x_stepps_pro_lage;
uint32_t y_stepps_pro_lage;
uint32_t y_stepps_per_x_stepps;

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);
void setupMenu()
{
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemWicklen);

  menuPageWickeln.setParentMenuPage(menuPageMain);
  menuPageWickeln.addMenuItem(menuItemManWicklen);
  menuPageWickeln.addMenuItem(menuItemAutoWicklen);
  menuPageWickeln.addMenuItem(menuItemWickelnKalibrieren);
  menuPageManWickeln.setParentMenuPage(menuPageWickeln);
  menuPageManWickeln.addMenuItem(menuItemJoystickPosX);

  menuPageAutoWickeln.setParentMenuPage(menuPageWickeln);
  menuPageAutoWickeln.addMenuItem(menueItemAutoLageLeft);
  menuPageAutoWickeln.addMenuItem(menueItemAutoLageRight);
  menuPageAutoWickeln.addMenuItem(menuItemWicklungen);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}
unsigned long prevMillis;
void setup(void)
{

  stepper_x.begin(RPM_X, MICROSTEPS);
  stepper_y.begin(RPM_Y, MICROSTEPS);
  stepper_z.begin(RPM_Z, MICROSTEPS);
  // menuItemInt.setReadonly();
  Serial.begin(9600); // Start serial communication at 9600 bps
  joystick.begin();   // Initialize the joystick
  endstop.begin();    // Initialize the endstop
  rotary_button.attach(ROT_BUTTON_PIN, INPUT_PULLUP);
  rotary_button.setPressedState(LOW);
  rotary_button.interval(2);
 // wicklung_pro_lage = SPULEN_Breite / FASER_DIAMETER;
 // x_stepps_pro_lage = SPULEN_Breite * (endstop.x_stepps / X_Lenght);
 // y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * wicklung_pro_lage;
 // y_stepps_per_x_stepps = y_stepps_pro_lage / x_stepps_pro_lage;
//
  pinMode(encoder0Btn, INPUT_PULLUP);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  pinMode(Z_STEP_PIN,OUTPUT);
  pinMode(Z_DIR_PIN,OUTPUT);
  pinMode(Z_ENABLE_PIN,OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);

  /*Rotarty Encoder*/
  qdec.begin();
  // FULL GRAPHIC SMART CONTROLLER BY: MWENE ADOLFO
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x12_tr);
  menu.init();
  setupMenu();
  menu.drawMenu();
}

void loop(void)
{

  joystick.button.update();                            // Polls the button
  rotary_button.update();                              // Polls the button
  endstop.update();                                    // Polls the endstop
  ::SimpleHacks::QDECODER_EVENT event = qdec.update(); // Polls the rotary encoder

  // Serial.print("loop");
  x = joystick.readX();
  y = joystick.readY();

  // increment for clockwise, decrement for counter-clockwise
  if (event & ::SimpleHacks::QDECODER_EVENT_CW)
  {
    menu.registerKeyPress(GEM_KEY_DOWN);
    // menu.drawMenu();
  }
  else if (event & ::SimpleHacks::QDECODER_EVENT_CCW)
  {
    menu.registerKeyPress(GEM_KEY_UP);
  }
  else if (rotary_button.pressed())
  {
    menu.registerKeyPress(GEM_KEY_OK);
  }

  // Kalibration
  if (joystick.button.released() && endstop.isCalibrated == false)
  {
    Serial.print("Calibrating");
    endstop.calibrate();
    joystick.button.update();
    if (endstop.isCalibrated == true)
    {
      Serial.print("Calibrated");
      Serial.print(endstop.x_stepps);
    }
  }

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
    // stepper_y.rotate(8);
    stepper_x.rotate(2);
    stepper_z.rotate(2);
    // Serial.print("x+");
  }
  else if (x < 400)
  {
    stepper_x.rotate(-2);
    stepper_z.rotate(-2);
    // stepper_y.rotate(8);
    //  Serial.print("x-");
  }
  else if (y > 600)
  {
    stepper_y.rotate(-2);
     
  }
  else if (y < 400)
  {
    stepper_y.rotate(2);
  };
}

void wickelLageLeft()
{
  uint16_t stepps_rotated = 0;
  while (stepps_rotated <= endstop.x_stepps_pro_lage)
  {
    stepper_y.move(endstop.y_stepps_per_x_stepps);
    stepper_x.move(-1);
    stepper_z.move(-1);
    stepps_rotated++;
  }
  wicklungen+=(stepps_rotated*endstop.y_stepps_per_x_stepps)/endstop.y_steps_per_rotation;
  menu.drawMenu();
}

void wickelLageRight()
{
  
  uint16_t stepps_rotated = 0;

  while (stepps_rotated < endstop.x_stepps_pro_lage)
  {
    
    stepper_y.move(endstop.y_stepps_per_x_stepps);
    stepper_x.move(1);
    stepper_z.move(1);
    stepps_rotated++; 
  }
  wicklungen+=(stepps_rotated*endstop.y_stepps_per_x_stepps)/endstop.y_steps_per_rotation;
  menu.drawMenu();
}
void kalibrieren()
{
  endstop.calibrate();
}