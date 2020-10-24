/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "deplacement.h"

#include "../Timer/TimerOne.h"


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define TIMER_DELAY_MS  10


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Deplacement_PID_Calculate();


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Deplacement_Init()
{
    Timer1.initialize(TIMER_DELAY_MS * 1000);
    Timer1.attachInterrupt(&Deplacement_PID_Calculate);

    pinMode(LED_BUILTIN, OUTPUT);
}

void Deplacement_PID_Calculate()
{
    static int counter = 0;
    static bool state = false;
    if (counter++ > 200)
    {
        digitalWrite(LED_BUILTIN, state = !state);
        counter = 0;
    }
}
