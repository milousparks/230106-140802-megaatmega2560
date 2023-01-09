#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#include "DRV8825.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


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

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 23 /* A4 */ , /* data=*/ 17 /* A2 */, /* CS=*/ 16 /* A3 */, /* reset=*/ U8X8_PIN_NONE);

/* Variablen definieren: */

// End of constructor list
/* Rotary encoder (dial) pins */
#define ROT_EN_A 57
#define ROT_EN_B 58

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

#define encoder0Btn 35
int encoder0Pos = 0;
//aktueller stand
int temp_state  = 0;
//letzter stand
int lbtn_state = 1;


unsigned long prevMillis;
void setup(void) {
  Serial.begin(9600);
  pinMode(encoder0Btn, INPUT_PULLUP);
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);

  digitalWrite(X_ENABLE_PIN    , LOW);
  digitalWrite(Y_ENABLE_PIN    , LOW);

  // FULL GRAPHIC SMART CONTROLLER BY: MWENE ADOLFO
  u8g2.begin(/* menu_select_pin= */ BUTTON_DIO, /* menu_next_pin= */ ROT_EN_B, /* menu_prev_pin= */ ROT_EN_A, /* menu_home_pin= */ KILL_PIN);
  u8g2.setFont(u8g2_font_6x12_tr);
}
void laufer(){
    if (millis() %2000 <1000) {
    digitalWrite(X_DIR_PIN    , LOW);
    digitalWrite(Y_DIR_PIN    , HIGH);
                      }
  else {
    digitalWrite(X_DIR_PIN    , LOW);
    digitalWrite(Y_DIR_PIN    , LOW);
    }
    digitalWrite(X_STEP_PIN    , HIGH);
    digitalWrite(Y_STEP_PIN    , HIGH);
    delay(1);
     
    digitalWrite(X_STEP_PIN    , LOW);
    digitalWrite(Y_STEP_PIN    , LOW);
}

void aufruf(){
      
      int btn = digitalRead(encoder0Btn);
      if ((btn == 0) && (lbtn_state == 1) && (temp_state == 0)) {
      temp_state = 1;
      Serial.print("\n ts");
      Serial.print(temp_state);// dient zur ürberprüfung des der variable
      //delay(500);
                                                                }

     btn = digitalRead(encoder0Btn);
     if ((btn == 0) && (lbtn_state == 1) && (temp_state == 1)) {
      temp_state = 0;
      Serial.print("\n ts");
      Serial.print(temp_state);
                                                                }
     if (temp_state == 1){
      laufer();
                         }                  
     lbtn_state = btn;
     Serial.print(lbtn_state);
                                                                                 
}
struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_state
{
  int16_t menu_start;    /* in pixel */
  int16_t frame_position;   /* in pixel */
  uint8_t position;     /* position, array index */
};

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

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_embedded_4x_t, 66, "Prog. Starten"}, 
  { u8g2_font_open_iconic_embedded_4x_t, 67, "Rueck. Fahren"},
  { u8g2_font_open_iconic_embedded_4x_t, 68, "Default"},
  { NULL, 0, NULL } 
};

void draw(struct menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while( menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
  }
  u8g2.drawFrame(state->frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
  u8g2.drawFrame(state->frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
  u8g2.drawFrame(state->frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
}


void to_right(struct menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
    }
  }
}

void to_left(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
      
    }
  }
}


uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}

uint8_t towards(struct menu_state *current, struct menu_state *destination)
{
  uint8_t r = 0;
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  return r;
}



struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };

void loop(void) {
  int8_t event;

  do
  {
    u8g2.clearBuffer();
    draw(&current_state);  
    u8g2.setFont(u8g2_font_helvB10_tr);  
    u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
    u8g2.print(menu_entry_list[destination_state.position].name);    
    u8g2.sendBuffer();
    delay(10);
    event = u8g2.getMenuEvent();
    if ( event == U8X8_MSG_GPIO_MENU_NEXT )
      to_right(&destination_state);
    if ( event == U8X8_MSG_GPIO_MENU_PREV )
      to_left(&destination_state);
    if ( event == U8X8_MSG_GPIO_MENU_SELECT )
    {
      u8g2.setFont(u8g2_font_helvB10_tr);  
      u8g2.userInterfaceMessage("Selection:", menu_entry_list[destination_state.position].name, "", " Ok ");
    }
  } while ( towards(&current_state, &destination_state) );
}
