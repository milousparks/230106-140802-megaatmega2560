#include "Settings.h"

Settings::Settings()
{ /*Init Settings with definitions*/
    this->spule_breite = SPULEN_Breite;
    this->faser_durchmesser = FASER_DIAMETER;
    this->xsteps=X_STEPS;
}

void Settings::begin()
{
    pinMode(encoder0Btn, INPUT_PULLUP);
    pinMode(X_STEP_PIN, OUTPUT);
    pinMode(X_DIR_PIN, OUTPUT);
    pinMode(X_ENABLE_PIN, OUTPUT);
    pinMode(Y_STEP_PIN, OUTPUT);
    pinMode(Y_DIR_PIN, OUTPUT);
    pinMode(Y_ENABLE_PIN, OUTPUT);
    digitalWrite(X_ENABLE_PIN, LOW);
    digitalWrite(Y_ENABLE_PIN, LOW);
    pinMode(Z_STEP_PIN, OUTPUT);
    pinMode(Z_DIR_PIN, OUTPUT);
    pinMode(Z_ENABLE_PIN, OUTPUT);
    digitalWrite(Z_ENABLE_PIN, LOW);
}

void Settings::calculateWindings()
{
    this->wicklung_pro_lage = this->spule_breite / this->faser_durchmesser;
    this->x_stepps_pro_lage = this->spule_breite * (xsteps / X_Lenght);
    this->y_steps_per_rotation = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS;
    this->y_stepps_pro_lage = MICROSTEPS * WICKELACHSE_UEBERSETZUNG * MOTOR_STEPS * this->wicklung_pro_lage;
    this->y_stepps_per_x_stepps = this->y_stepps_pro_lage / this->x_stepps_pro_lage;
}
