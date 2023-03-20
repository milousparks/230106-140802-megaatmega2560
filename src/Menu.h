#ifndef MENU_H
#define MENU_H
#include "definitions.h"
#include <qdec.h>
#include <GEM_u8g2.h>
#include <String.h>
#include <U8g2lib.h>

#pragma once
//
//class Menu
//{
//public:
//    Menu();
//    ~Menu();
//    void setupMenu();
//    GEM_u8g2 menu(u8g2);
//    U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/23 /* A4 */, /* data=*/17 /* A2 */, /* CS=*/16 /* A3 */, /* reset=*/U8X8_PIN_NONE);
//
//    ::SimpleHacks::QDecoder qdec(int rot_pin_a, int rot_pin_b, bool setting);
//    GEMPage menuPageMain(string main_menu);
//    GEMPage menuPageWickeln("Wickeln");
//    GEMPage menuPageManWickeln("Manuell Wickeln");
//    GEMPage menuPageAutoWickeln("Automatisch Wickeln");
//    GEMPage menuPageSettings("Settings");
//
//    GEMItem menuItemWicklen("Wickeln", menuPageWickeln);
//    GEMItem menuItemManWicklen("Manuell Wickeln", menuPageManWickeln);
//    GEMItem menuItemAutoWicklen("Automatisch Wickeln", menuPageAutoWickeln);
//    GEMItem menuItemSettings("Settings", menuPageSettings);
//    GEMItem menuItemJoystickPosX("Joystick Pos. X:", x);
//    GEMItem menueItemAutoLageLeft("Wickel 1 Lage <-", wickelLageLeft);
//    GEMItem menueItemAutoLageRight("Wickel 1 Lage ->", wickelLageRight);
//    GEMItem menuItemWicklungen("Windungen:", wicklungen);
//    GEMItem menuItemWickelnKalibrieren("Kalibrieren", kalibrieren);
//
//    GEMItem menuItemSettingsSpuleBreite("Spulen Breite:", settings.spule_breite);
//    GEMItem menuItemSettingsFaserDurchmesser("Faser Durchmesser:", settings.faser_durchmesser);
//
//private:
//};
//
#endif