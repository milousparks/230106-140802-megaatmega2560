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
#include "Settings.h"
#include <OneButton.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#include "DRV8825.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, LCD_D4_CLOCK, LCD_EN_DATA, LCD_CS, /* reset=*/U8X8_PIN_NONE);

/*Joystick*/
int x;
uint16_t y;
/* Klassen Def*/
DRV8825 stepper_x(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
DRV8825 stepper_y(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
DRV8825 stepper_z(MOTOR_STEPS, Z_DIR_PIN, Z_STEP_PIN, Z_ENABLE_PIN);
AnalogJoystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Endstop endstop(X_MIN_PIN, X_MAX_PIN, &stepper_x, &stepper_y);
::SimpleHacks::QDecoder qdec(ROT_EN_A, ROT_EN_B, true);
OneButton rotary_button(ROT_BUTTON_PIN, true);
Settings settings;
void (Settings::*calcSet)() = &Settings::calculateWindings;
float wicklungen;

/*Forward Declarations*/
void printData();
void wickelLageLeft();
void wickelLageRight();
void kalibrieren();
void rotary_button_clicked();
void rotary_button_long_press_start();
void rotary_button_double_clicked();

/*Menu Pages*/
GEMPage menuPageMain("Main Menu");
GEMPage menuPageWickeln("Wickeln");
GEMPage menuPageManWickeln("Manuell Wickeln");
GEMPage menuPageAutoWickeln("Automatisch Wickeln");
GEMPage menuPageSettings("Settings");

/*Menu Items*/
GEMItem menuItemWicklen("Wickeln", menuPageWickeln);
GEMItem menuItemManWicklen("Manuell Wickeln", menuPageManWickeln);
GEMItem menuItemAutoWicklen("Automatisch Wickeln", menuPageAutoWickeln);
GEMItem menuItemSettings("Settings", menuPageSettings);
GEMItem menuItemJoystickPosX("Joystick Pos. X:", x);
GEMItem menueItemAutoLageLeft("Wickel 1 Lage <-", wickelLageLeft);
GEMItem menueItemAutoLageRight("Wickel 1 Lage ->", wickelLageRight);
GEMItem menuItemWicklungen("Windungen:", wicklungen);
GEMItem menuItemWickelnKalibrieren("Kalibrieren", kalibrieren);
GEMItem menuItemSettingsSpuleBreite("Spulen Breite:", settings.spule_breite, calcSet);
GEMItem menuItemSettingsFaserDurchmesser("Faser Durchmesser:", settings.faser_durchmesser, calcSet);

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);
void setupMenu()
{
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemWicklen);
  menuPageMain.addMenuItem(menuItemSettings);

  // Items for MenuPage Manuell Wickeln
  menuPageWickeln.setParentMenuPage(menuPageMain);
  menuPageWickeln.addMenuItem(menuItemManWicklen);
  menuPageWickeln.addMenuItem(menuItemAutoWicklen);
  menuPageWickeln.addMenuItem(menuItemWickelnKalibrieren);
  menuPageManWickeln.setParentMenuPage(menuPageWickeln);
  menuPageManWickeln.addMenuItem(menuItemJoystickPosX);

  // Items for MenuPage Automatisch Wickeln
  menuPageAutoWickeln.setParentMenuPage(menuPageWickeln);
  menuPageAutoWickeln.addMenuItem(menueItemAutoLageLeft);
  menuPageAutoWickeln.addMenuItem(menueItemAutoLageRight);
  menuPageAutoWickeln.addMenuItem(menuItemWicklungen);

  // Items for MenuPage Settings
  menuPageSettings.setParentMenuPage(menuPageMain);
  menuPageSettings.addMenuItem(menuItemSettingsSpuleBreite);
  menuPageSettings.addMenuItem(menuItemSettingsFaserDurchmesser);

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

  rotary_button.attachClick(rotary_button_clicked);
  rotary_button.attachDoubleClick(rotary_button_double_clicked);
  rotary_button.attachLongPressStart(rotary_button_long_press_start);
  // Set Pinmodes
  settings.begin();

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

  joystick.button.update(); // Polls the button
                            // Polls the button
  rotary_button.tick();
  endstop.update();                                    // Polls the endstop
  ::SimpleHacks::QDECODER_EVENT event = qdec.update(); // Polls the rotary encoder

  // Serial.print("loop");
  x = joystick.readX();
  y = joystick.readY();

  // increment for clockwise, decrement for counter-clockwise
  if (event & ::SimpleHacks::QDECODER_EVENT_CW)
  {
    menu.registerKeyPress(GEM_KEY_UP);
  }
  else if (event & ::SimpleHacks::QDECODER_EVENT_CCW)
  {
    menu.registerKeyPress(GEM_KEY_DOWN);
  }

  if (x > 600)
  {
    stepper_x.rotate(2);
    stepper_z.rotate(2);
  }
  else if (x < 400)
  {
    stepper_x.rotate(-2);
    stepper_z.rotate(-2);
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
  uint16_t steps_rotated = 0;
  while (steps_rotated <= endstop.x_steps_pro_lage)
  {
    stepper_y.move(endstop.y_steps_per_x_steps);
    stepper_x.move(-1);
    stepper_z.move(-1);
    steps_rotated++;
  }
  wicklungen += (steps_rotated * endstop.y_steps_per_x_steps) / endstop.y_steps_per_rotation;
  menu.drawMenu();
}

void wickelLageRight()
{

  uint16_t steps_rotated = 0;

  while (steps_rotated <= endstop.x_steps_pro_lage)
  {

    stepper_y.move(endstop.y_steps_per_x_steps);
    stepper_x.move(1);
    stepper_z.move(1);
    steps_rotated++;
  }
  wicklungen += (steps_rotated * endstop.y_steps_per_x_steps) / endstop.y_steps_per_rotation;
  menu.drawMenu();
}
void kalibrieren()
{
  settings.xsteps = endstop.calibrate();
}

void rotary_button_clicked()
{
  menu.registerKeyPress(GEM_KEY_OK);
}

void rotary_button_double_clicked()
{
  menu.registerKeyPress(GEM_KEY_LEFT);
}

void rotary_button_long_press_start()
{
  menu.registerKeyPress(GEM_KEY_RIGHT);
}


