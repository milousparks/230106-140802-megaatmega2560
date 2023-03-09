/* Stepper Motoren*/

// 200 Stepp Motor
 #define MOTOR_STEPS 200
 #define MICROSTEPS 32
 #define RPM_X 60
 #define RPM_Y 60
/* Anschlussbelegung für die X-Achse: */
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#ifndef X_CS_PIN
  #define X_CS_PIN         53
#endif
/* Anschlussbelegung für die Y-Achse: */
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#ifndef Y_CS_PIN
  #define Y_CS_PIN         49
#endif

/* Hardware Übersetzungen*/
#define WICKELACHSE_UEBERSETZUNG 3
/* Pins definieren: */

/* Joystick*/
#define JOYSTICK_X 4
#define JOYSTICK_Y 3
#define JOYSTICK_SW 1
// End of constructor list
/* Rotary encoder (dial) pins */
#define ROT_EN_A 31
#define ROT_EN_B 33

/* Rotary encoder button pin */
#define BUTTON_DIO 35
#define BUZZER_DIO 37
#define KILL_PIN 41

/* LCD display pins */
#define LCD_RS 16
#define LCD_EN 17
#define LCD_D4 23
#define LCD_D5 25
#define LCD_D6 27
#define LCD_D7 29

#define encoder0Btn 2
int encoder0Pos = 0;
//aktueller stand
int temp_state  = 0;
//letzter stand
int lbtn_state = 1;

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32+ ICON_GAP

// Endstops

#define X_MIN_PIN 14
#define X_MAX_PIN 15
